#ifndef LINEARELASTICINTEGRATOR_H
#define LINEARELASTICINTEGRATOR_H

#include "mfem.hpp"
#include "Tensor2Voigt.hpp"

namespace mfem {
/// Linear Elastic Integrator of the form grad_v:C:grad_u (\nabla \bm{v}:\bm{C}:\nabla \bm{u}).
/// Here grad_u is of the form [e_11 e_22 e_33 2*e_12 2*e_13 2*e23]^T where 'e' represents strain tensor.
class LinearElasticIntegrator: public mfem::BilinearFormIntegrator
{
public:
    LinearElasticIntegrator(MatrixCoefficient *C);
    virtual void AssembleElementMatrix(const FiniteElement &,
                                       ElementTransformation &,
                                       DenseMatrix &);
protected:
   MatrixCoefficient *C_tan;

private:
#ifndef MFEM_THREAD_SAFE
   Vector shape;
   DenseMatrix dshape, gshape;
   std::map<int, int> index;
   Tensor2Voigt Elstcty_Tensor;
#endif

};

} // namespace mfem


#endif // LINEARELASTICINTEGRATOR_H
