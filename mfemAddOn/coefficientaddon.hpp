#ifndef COEFFICIENTADDON_HPP
#define COEFFICIENTADDON_HPP

#include "mfem.hpp"

namespace  mfem
{
class PWMatrixCoefficient: public MatrixCoefficient
{
public:
    DenseMatrix *mat;
    PWMatrixCoefficient(int dim_square, int max_Attributes);
    virtual void Eval(DenseMatrix &K, ElementTransformation &T,
                      const IntegrationPoint &ip);
    ~PWMatrixCoefficient()
    {
        delete [] mat;
    }
protected:


};
}


#endif // COEFFICIENTADDON_HPP
