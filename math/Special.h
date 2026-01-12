#pragma once

#include "Math.h"

namespace Ath::Math
{
    /**
     * @brief Dirichlet kernel approximation.
     */
    template <typename T>
    static T dirichlet(T x, int n) noexcept
    {
        const auto x1 = foldArgument(x);

        const auto cos = sin2pi9(x1 + 0.25);

        if (abs(x1) < 0.01) return cos;

        return sin2pi9(x1 * n) / (x * T(n) * pi<T>);
    }

    /**
     * @brief Laтczos kernel with a=3
     * 
     */
    template <typename T>
    static T lanczos3(T x) noexcept
    {
        static const T a = 3.0;
        static const T ainv = 1.0 / a;

        const auto pix = x * pi<T>;

        const auto sina = sin(pix * ainv);
        const auto sin = chebyshev3(sina);

        return (a * sin * sina) / (pix * pix);
    }
}