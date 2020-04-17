#include "Tensor2Voigt.hpp"

Tensor2Voigt::Tensor2Voigt()
{
    // Since the problem is solved using tensor notation, but in reality is
    // converted to matrices using voigt noation, the tensor notations have to be mapped
    // to vector positions. This mapping is stored below.
    // Map for Stiffness Elastic Tensor.
    index.insert(std::pair<int, int>(00, 0));
    index.insert(std::pair<int, int>(11, 1));
    index.insert(std::pair<int, int>(22, 2));
    index.insert(std::pair<int, int>(01, 3));
    index.insert(std::pair<int, int>(02, 4));
    index.insert(std::pair<int, int>(12, 5));
}

void Tensor2Voigt::GetVoigtIndex(const int &i, const int &j, const int &k, const int &l, int &ij, int &kl)
{
    ij = index.find(10*i+j)->second;
    kl = index.find(10*k+l)->second;
}
