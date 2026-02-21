#include "external/CryptoAPI.h"

void CryptoAPI::transfer(const std::string& wallet, double amount) {
    std::cout << "[CryptoAPI] transfer to wallet=" << wallet
              << " amount=" << amount << "\n";
}