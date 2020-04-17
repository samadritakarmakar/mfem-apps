#include "CalcStressSolids.hpp"
#include "../mfem/linalg/vector.hpp"
namespace mfem
{
CalcStressSolids::CalcStressSolids(FiniteElementSpace *fe_disp,  const GridFunction &x,
                                   MatrixCoefficient *C_tan, FiniteElementSpace *fe_stress):
    GridFunction(fe_stress), mesh(fe_stress->GetMesh()), fe_dsp(fe_disp), u(x), C_tan(C_tan), fe_strss(fe_stress)
{
    InitializeToZero();
    sum=Vector(fe_strss->GetNDofs());
    sum =0.;
    inv_distance =Vector(fe_strss->GetNDofs());
    int noOfElmnts = fe_dsp->GetNE();
    //Delete later*************************
    MaxStrss_anyIpnt_anyDim = Vector (6);
    MaxStrss_anyIpnt_anyDim=0.;
    //************************************
    for (int elmnt =0; elmnt<noOfElmnts; elmnt++)
    {
        const FiniteElement *el_dsp = fe_dsp->GetFE(elmnt);
        const FiniteElement *el_strss = fe_strss->GetFE(elmnt);
        Array<int> u_vdof;
        Array<int> strss_vdof;
        Array<int> inv_dof;
        Array<Vector> strss_elmnt;
        Array<Vector> inv_elmnt_dist;
        fe_dsp->GetElementVDofs(elmnt, u_vdof);
        fe_strss->GetElementVDofs(elmnt, strss_vdof);
        fe_strss->GetElementDofs(elmnt, inv_dof);
        Vector u_elmnt;
        u.GetSubVector(u_vdof, u_elmnt);
        ElementTransformation *Trans=fe_dsp->GetElementTransformation(elmnt);
        IsoparametricTransformation IsoTransformTemp;
        fe_dsp->GetElementTransformation(elmnt, &IsoTransformTemp);
        IsoTransform =&IsoTransformTemp;
        DenseMatrix PointMat=IsoTransform->GetPointMat();
        AssembleElementVect(el_dsp, Trans, u_elmnt, strss_elmnt);
        GetInverseDistance(PointMat, el_dsp, Trans, inv_elmnt_dist);
        int strssDof =el_strss->GetDof();
        strssDim =strss_elmnt[0].Size();
        //************Delete Later****************
        CalcMaxStrss_anyIpnt_anyDim(strss_elmnt);
        //****************************************
        Vector  stressPerElmnt(strssDof*strssDim);
        stressPerElmnt=0.;
        Vector  sumPerDof(strssDof);
        sumPerDof=0.;
        Vector stressPerDof(strssDim);
        for (int dof_cnt=0; dof_cnt<strssDof; dof_cnt++)
        {
            stressPerDof=0.;
            for (int ipnt=0; ipnt<strss_elmnt.Size(); ipnt++)
            {
                stressPerDof.Add(inv_elmnt_dist[ipnt](dof_cnt), strss_elmnt[ipnt]);
                sumPerDof(dof_cnt) +=  inv_elmnt_dist[ipnt](dof_cnt);
            }
            swapStress_13_23(stressPerDof);
            for (int dim_cnt=0; dim_cnt<strssDim; dim_cnt++)
            {

                stressPerElmnt(strssDof*dim_cnt+dof_cnt) += stressPerDof(dim_cnt);
            }
        }
        AddElementVector(strss_vdof, stressPerElmnt);
        sum.AddElementVector(inv_dof, sumPerDof);
    }
    Finalize();
    std::cout<<"Max stress in any dim and in any dof is: \n";
    //********************Delete later*******************************
    for (int dim=0; dim<strssDim; dim++)
    {
        std::cout<<MaxStrss_anyIpnt_anyDim(dim)<<"\n";
    }
    //********************Delete later*******************************
}


void CalcStressSolids::AssembleElementVect(const FiniteElement *el, ElementTransformation *Trans, const Vector& u,
                                                Array<Vector> &strss)
{

    int dof = el->GetDof();
    int dim = el->GetDim();
    DenseMatrix C;

    MFEM_ASSERT(dim == Trans.GetSpaceDim(), "");

 #ifdef MFEM_THREAD_SAFE
    DenseMatrix dshape(dof, dim), gshape(dof, dim);
 #else
    dshape.SetSize(dof, dim);
    gshape.SetSize(dof, dim);
 #endif
    const IntegrationRule *ir = GetIntegrationRule(el, Trans);
    strss.SetSize(ir -> GetNPoints());
    for (int ipnt = 0; ipnt < ir -> GetNPoints(); ipnt++)
    {
       const IntegrationPoint &ip = ir->IntPoint(ipnt);

       el->CalcDShape(ip, dshape);

       Trans->SetIntPoint(&ip);
       Mult(dshape, Trans->InverseJacobian(), gshape);
       C_tan->Eval(C, *Trans, ip);
       strss[ipnt].SetSize(C.Height());
       strss[ipnt] = 0.;
       for (int j = 0; j < dim; j++)
       {
          for (int i = 0; i <= j; i++)
          {
             for (int l = 0; l < dim; l++)
             {
                 for (int k = 0; k <= l; k++)
                 {
                     //This is to set the coefficient because e_kl = 0.5*(grad_u + grad_u^T) when k==l but
                     // for shear strains, when k not = k, e_lk = (grad_u + grad_u^T).
                     double coeff = (k == l) ? 0.5 : 1.0;
                     int ij, kl;
                     Elstcty_Tensor.GetVoigtIndex(i,j,k,l, ij, kl);
                     for (int b =0; b < dof && C(ij,kl); b++) //Hopefully being able to skip zeros will speed up overall process.
                     {
                         strss[ipnt](ij) += coeff*C(ij,kl)*gshape(b,l)*u(dof*k+b);
                         strss[ipnt](ij) += coeff*C(ij,kl)*gshape(b,k)*u(dof*l+b);
                     }
                 }
             }
          }
       }
    }
}



void CalcStressSolids::GetInverseDistance(const DenseMatrix& PointMat, const FiniteElement *el, ElementTransformation *Trans, Array<Vector> &InvDist)
{
    const IntegrationRule *ir = GetIntegrationRule(el, Trans);
    int dim = el->GetDim();
    Vector trans_vctr;
    Vector Dist;
    Dist.SetSize(dim);
    InvDist.SetSize(ir -> GetNPoints());
    for (int ipnt = 0; ipnt < ir -> GetNPoints(); ipnt++)
    {
        const IntegrationPoint &ip = ir->IntPoint(ipnt);
        Trans->Transform(ip, trans_vctr);
        InvDist[ipnt].SetSize(PointMat.Width());
        InvDist[ipnt]=0.;
        for (int dof_cnt=0; dof_cnt<PointMat.Width(); dof_cnt++)
        {
            for (int dim_cnt=0; dim_cnt<dim; dim_cnt++)
            {
                Dist(dim_cnt) = PointMat(dim_cnt, dof_cnt) - trans_vctr(dim_cnt);
            }
            InvDist[ipnt](dof_cnt)=1./(std::pow(Dist.Norml2(),12.));
        }
    }
}

void CalcStressSolids::Finalize()
{
    int dof=sum.Size();
    for (int DofNo =0; DofNo<dof; DofNo++)
    {
        for (int dimNo=0; dimNo<strssDim; dimNo++)
        {
            this->operator()(dof*dimNo+DofNo) = this->operator()(dof*dimNo+DofNo)/sum(DofNo);
        }

    }
}

void CalcStressSolids::InitializeToZero()
{
    for (int i=0; i<this->Size(); i++)
    this->operator()(i)=0.;
}

void CalcStressSolids::CalcMaxStrss_anyIpnt_anyDim(Array<Vector> &strss_elmnt)
{
    for (int ipnt=0; ipnt<strss_elmnt.Size(); ipnt++)
    {
        for (int dim=0; dim<strssDim; dim++)
        {
            //std::cout<<"At ipnt "<<ipnt<<" & dim "<<dim<<" ";
            MaxStrss_anyIpnt_anyDim(dim) = strss_elmnt[ipnt](dim)> MaxStrss_anyIpnt_anyDim(dim) ?
                        strss_elmnt[ipnt](dim) : MaxStrss_anyIpnt_anyDim(dim);
        }
        //std::cout<<"\n";
    }

}

void CalcStressSolids::swapStress_13_23(Vector &stressPerDof)
{
    double TempVariable;
    int size = stressPerDof.Size();
    TempVariable=stressPerDof(size-2);
    stressPerDof(size-2)=stressPerDof(size-1);
    stressPerDof(size-1)=TempVariable;
}

const IntegrationRule* CalcStressSolids::GetIntegrationRule(const FiniteElement *el, ElementTransformation *Trans)
{
    const IntegrationRule *ir = IntRule;
    if (ir == NULL)
    {
       int order = 2 * Trans->OrderGrad(el); // correct order?
       ir = &IntRules.Get(el->GetGeomType(), order);
    }
    return  ir;
}



}//end of namespace mfem

