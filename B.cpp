#include <iostream>
#include <vector>

std::vector<int> BuildSmallestPrimeFactorTable(int n) {
    std::vector<int> spf(n + 1, 0);

    for (int p = 2; p <= n; ++p) {
        if (spf[p] == 0) {
            for (long long multiple = 1LL * p * 2; multiple <= n; multiple += p) {
                if (spf[static_cast<int>(multiple)] == 0) {
                    spf[static_cast<int>(multiple)] = p;
                }
            }
        }
    }

    return spf;
}

long long SumSmallestPrimeFactors(const std::vector<int>& spf) {
    long long total = 0;
    for (int value : spf) {
        total += value;
    }
    return total;
}

int main() {
    int size;
  
    if (!(std::cin >> size) || size < 0) {
        return 0;
    }

    const auto spf = BuildSmallestPrimeFactorTable(size);
    const long long result = SumSmallestPrimeFactors(spf);

    std::cout << result;
    return 0;
}
