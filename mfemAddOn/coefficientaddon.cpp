#include "coefficientaddon.hpp"
#include <vector>
namespace mfem
{
PWMatrixCoefficient::PWMatrixCoefficient(int dim_square, int max_Attributes):
    MatrixCoefficient(dim_square)
{
    mat = new DenseMatrix[max_Attributes];
    for (int i=0;i<max_Attributes;i++)
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
