#include <iostream>
#include <iomanip>

#include "crypto.h"

int main(int argc, char **argv)
{
    std::vector<uint8_t> input = {'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!'};
    auto dataE = CryptedVault::CryptoUtils::EncryptAES256(input, "tester@jaj_a!");
    auto dataD = CryptedVault::CryptoUtils::DecryptAES256(dataE, "tester@jaj_a!");

    std::cout << "original:\t";
    for (uint8_t byteI: input)
        std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (int)byteI << " ";
    std::cout << std::endl << "encrypted:\t";
    for (uint8_t byteE: dataE)
        std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (int)byteE << " ";
    std::cout << std::endl << "decrypted:\t";
    for (uint8_t byteD: dataD)
        std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << (int)byteD << " ";
    std::cout << std::endl;

    return 0;
}