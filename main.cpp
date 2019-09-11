#include <algorithm>
#include <array>
#include <complex>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Disable synchronization between the C and C++ standard streams.
static const int __ = []() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    return 0;
}();

using Complex = std::complex<double>;

std::vector<Complex>
dft(std::vector<double> const & x)
{
    std::vector<Complex> X(x.size(), 0);

    size_t const N = x.size();

    for (size_t k = 0; k != x.size(); ++k)
    {
        std::thread t{
            [&x, &X, k, N]()
            {
                Complex sum = 0;
                for (size_t n = 0; n != x.size(); ++n)
                {
                    sum += x[n] * (cos(2 * M_PI * k * n / N) - Complex{0, 1} * sin(2 * M_PI * k * n / N));
                }
                X[k] = sum;
            }};
        t.detach();
    }
    return X;
}

int main(int argc, char * argv[])
{
    std::vector<double> x = {1, 2, 3};
    for (auto val : x)
    {
        std::cout << val << std::endl;
    }

    auto X = dft(x);
    for (auto val : X)
    {
        std::cout << val << std::endl;
    }

    return EXIT_SUCCESS;
}
