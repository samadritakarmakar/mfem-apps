#include "GetElasticityTensor.hpp"
#include "mfem.hpp"
void GetElasticityTensor(const double E, const double nu, mfem::DenseMatrix &C)
{
    C=0.;
    C(0,0) = 1.-nu;  C(0,1) = nu;    C(0,2) = nu;
    C(1,0) = nu;    C(1,1) = 1.-nu;  C(1,2) = nu;
    C(2,0) = nu;    C(2,1) = nu;    C(2,2) = 1.-nu;
        C(3,3) = (1.-2.*nu)/2.;
            C(4,4) = (1.-2.*nu)/2.;
                C(5,5) = (1.-2.*nu)/2.;
    C.Set(E/((1.+nu)*(1.-2.*nu)), C);
}
