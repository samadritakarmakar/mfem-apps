#ifndef COEFFICIENTADDON_HPP
#define COEFFICIENTADDON_HPP

#include "mfem.hpp"

namespace  mfem
{
class PWVectorCoefficient: public VectorCoefficient
{
public:
    PWVectorCoefficient(int dim, int max_Attributes);
    virtual void Eval(Vector &vec, ElementTransformation &T,
                      const IntegrationPoint &ip);
    ~PWVectorCoefficient()
    {
        delete [] vec;
    }

    Vector& operator[](unsigned int index)
    {
        return vec[index];
    }

    void operator=(double value)
    {
        for(unsigned int i=0; i<max_attributes; i++)
        {
            vec[i] = value;
        }
    }
protected:
    Vector *vec;
    unsigned int max_attributes;
};

class PWMatrixCoefficient: public MatrixCoefficient
{
public:
    PWMatrixCoefficient(int dim_square, int max_Attributes);
    virtual void Eval(DenseMatrix &K, ElementTransformation &T,
                      const IntegrationPoint &ip);
    ~PWMatrixCoefficient()
    {
        delete [] mat;
    }
    DenseMatrix& operator[](unsigned int index)
    {
        return mat[index];
    }

    void operator=(double value)
    {
        for(unsigned int i=0; i<max_attributes; i++)
        {
            mat[i] = value;
        }
    }
protected:
    DenseMatrix *mat;
    unsigned int max_attributes;
};
}


#endif // COEFFICIENTADDON_HPP
