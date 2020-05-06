#include "coefficientaddon.hpp"
#include <vector>
namespace mfem
{
PWVectorCoefficient::PWVectorCoefficient(int dim, int max_Attributes): VectorCoefficient(dim), max_attributes(max_Attributes)
{
    vec = new Vector[max_Attributes];
    for (int i=0; i<max_Attributes; i++)
    {
        vec[i].SetSize(dim);
    }
}
void PWVectorCoefficient::Eval(Vector &V, ElementTransformation &T, const IntegrationPoint &ip)
{
    int currentAttrib = T.Attribute-1;
    V=vec[currentAttrib];
}


PWMatrixCoefficient::PWMatrixCoefficient(int dim_square, int max_Attributes):
    MatrixCoefficient(dim_square)
{
    mat = new DenseMatrix[max_Attributes];
    for (int i=0; i<max_Attributes; i++)
    {
        mat[i].SetSize(dim_square,dim_square);
    }
}
void PWMatrixCoefficient::Eval(DenseMatrix &K, ElementTransformation &T,
                          const IntegrationPoint &ip)
{
    int currentAttrib = T.Attribute-1;
    K=mat[currentAttrib];
}
}
