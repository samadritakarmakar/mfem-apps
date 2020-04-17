#include "LinearElasticIntegrator.hpp"
#include <map>

namespace mfem
{
LinearElasticIntegrator::LinearElasticIntegrator(MatrixCoefficient* C):C_tan(C)
{
}

void LinearElasticIntegrator::AssembleElementMatrix(
        const FiniteElement &el, ElementTransformation &Trans, DenseMatrix &elmat)
{
   int dof = el.GetDof();
   int dim = el.GetDim();
   double w;
   DenseMatrix C;

   MFEM_ASSERT(dim == Trans.GetSpaceDim(), "");

#ifdef MFEM_THREAD_SAFE
   DenseMatrix dshape(dof, dim), gshape(dof, dim);
#else
   dshape.SetSize(dof, dim);
   gshape.SetSize(dof, dim);
#endif

   elmat.SetSize(dof * dim);

   const IntegrationRule *ir = IntRule;
   if (ir == NULL)
   {
      int order = 2 * Trans.OrderGrad(&el); // correct order?
      ir = &IntRules.Get(el.GetGeomType(), order);
   }

   elmat = 0.0;

   for (int i = 0; i < ir -> GetNPoints(); i++)
   {
      const IntegrationPoint &ip = ir->IntPoint(i);

      el.CalcDShape(ip, dshape);

      Trans.SetIntPoint(&ip);
      w = ip.weight * Trans.Weight();
      Mult(dshape, Trans.InverseJacobian(), gshape);
      C_tan->Eval(C, Trans, ip);
      for (int j = 0; j < dim; j++)
      {
         for (int i = 0; i <= j; i++)
         {
            for (int l = 0; l < dim; l++)
            {
                for (int k = 0; k <= l; k++)
                {
                    //This is to set the coefficient because e_ij = 0.5*(grad_u + grad_u^T) when i==j but
                    // for shear strains, when i not = j, e_ij = (grad_u + grad_u^T).
                    //This test has to be done twice, for i & j for the test function and for k & l,
                    // the trial function.
                    double coeff= (i == j) ? 0.5 : 1.0;
                    coeff *= (k == l) ? 0.5 : 1.0;
                    int ij, kl;
                    Elstcty_Tensor.GetVoigtIndex(i,j,k,l, ij, kl);
                    for (int b =0; b < dof && C(ij,kl); b++) //Hopefully being able to skip zeros will speed up overall process.
                    {
                        for (int a =0; a < dof; a++)
                        {
                            elmat(dof*i+a, dof*k+b) += w*coeff*gshape(a,j)*C(ij,kl)*gshape(b,l);
                            elmat(dof*i+a, dof*l+b) += w*coeff*gshape(a,j)*C(ij,kl)*gshape(b,k);
                            elmat(dof*j+a, dof*k+b) += w*coeff*gshape(a,i)*C(ij,kl)*gshape(b,l);
                            elmat(dof*j+a, dof*l+b) += w*coeff*gshape(a,i)*C(ij,kl)*gshape(b,k);
                        }
                    }
                }
            }
         }
      }
   }
}

}//End of namespace mfem
