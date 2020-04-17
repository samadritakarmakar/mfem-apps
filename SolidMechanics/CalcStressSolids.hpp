#ifndef CALCSTRESSSOLIDS_HPP
#define CALCSTRESSSOLIDS_HPP
#include "mfem.hpp"
#include "Tensor2Voigt.hpp"

namespace mfem
{
class CalcStressSolids: public GridFunction
{
public:
    CalcStressSolids(FiniteElementSpace *fe_disp, const GridFunction &x,
                     MatrixCoefficient *C_tan, FiniteElementSpace *fe_stress);
    void Finalize();
    void CalcMaxStrss_anyIpnt_anyDim(Array<Vector> &strss_elmnt);
protected:

    const Mesh *mesh;
    FiniteElementSpace *fe_dsp;
    const GridFunction& u;
    Vector stress, sum, inv_distance;
    MatrixCoefficient *C_tan;
    FiniteElementSpace *fe_strss;
    Tensor2Voigt Elstcty_Tensor;
    void AssembleElementVect(const FiniteElement *el_disp, ElementTransformation *Trans,
                             const Vector &u, Array<Vector> &strss);


    Vector shape;
    DenseMatrix dshape, gshape;
    const IntegrationRule *IntRule=NULL;
    IsoparametricTransformation *IsoTransform;

    void GetInverseDistance(const DenseMatrix& PointMat, const FiniteElement *el, ElementTransformation *Trans, Array<Vector> &InvDist);

private:
    int strssDim;
    Vector MaxStrss_anyIpnt_anyDim;
    void InitializeToZero();
    void swapStress_13_23(Vector& stressPerDof);
    const IntegrationRule *GetIntegrationRule(const FiniteElement *el, ElementTransformation *Trans);

};
}

#endif // CALCSTRESSSOLIDS_HPP
