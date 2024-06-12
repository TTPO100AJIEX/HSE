#include <iostream>
#include <limits>
#include <bitset>
#include <math.h>

int main()
{
    float a = std::numeric_limits<float>::infinity();
    a = pow(2, 126) * (1 - 1 / pow(2, 23));
    a = 1. / 8;
    a = -1 / pow(2, 148);
    a = -5;

    unsigned int rep = reinterpret_cast<unsigned int&>(a);
    std::cout << "16: " << std::hex << rep << std::endl;
    std::cout << "8: " << std::oct << rep << std::endl;
    std::cout << "2: " << std::bitset<32>(rep) << std::endl;

    std::cout << "man: " << std::bitset<32 - 8 - 1>(rep) << std::endl;
    rep >>= (32 - 8 - 1);
    
    std::cout << "exp: " << std::bitset<8>(rep) << std::endl;
    rep >>= 8;
    
    std::cout << "sign: " << std::bitset<1>(rep) << std::endl;
    return 0;
}