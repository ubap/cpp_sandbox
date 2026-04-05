#include <iostream>
#include <cassert>
#include <cmath>
#include <iomanip>

/*
 *
 * This method is part of LibreOffice project.
 * I isolated part of the code for experiments.
 * sc/source/core/data/table4.cxx:199
 */
double approxDiff(double a, double b) {
    if (a == b)
        return 0.0;
    if (a == 0.0)
        return -b;
    if (b == 0.0)
        return a;
    const double c = a - b;
    const double aa = fabs(a);
    const double ab = fabs(b);
    if (aa < 1e-16 || aa > 1e+16 || ab < 1e-16 || ab > 1e+16)
        // This is going nowhere, live with the result.
        return c;

    const double q = aa < ab ? b / a : a / b;
    const double d = (a * q - b * q) / q;
    if (d == c)
        // No differing error, live with the result.
        return c;

    // jtrzebiatowski:
    // Magic valuee to prove the program behaves differently.
    // I didn't bother copying rtl::math::round
    return 9999.0;
}

/*
 The following output on Macbook M4 (ARM64)

jtrzebiatowski@Mac-02490 sandbox % clang++ -std=c++20 -ffp-contract=off main.cpp -o test_arm_ffp_contract_off && ./test_arm_ffp_contract_off
a=1.10000000000000008882 | b=1.00000000000000000000 | a-b=9999.00000000000000000000

jtrzebiatowski@Mac-02490 sandbox % clang++ -std=c++20 -ffp-contract=on main.cpp -o test_arm_ffp_contract_on && ./test_arm_ffp_contract_on
a=1.10000000000000008882 | b=1.00000000000000000000 | a-b=0.10000000000000008882
 */
int main() {
    double a = 1.1;
    double b = 1;
    double c = approxDiff(1.1, 1);
    fprintf(stderr, "a=%.20f | b=%.20f | a-b=%.20f\n", a, b, c);
}
