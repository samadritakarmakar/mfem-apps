#ifndef TENSOR2VOIGT_HPP
#define TENSOR2VOIGT_HPP
#include <map>

class Tensor2Voigt
{
public:
    Tensor2Voigt();
    void GetVoigtIndex(const int &i, const int &j, const int &k, const int &l, int &ij, int &kl);
protected:
    std::map<int, int> index;
};

#endif // TENSOR2VOIGT_HPP
