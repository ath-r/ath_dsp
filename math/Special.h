#pragma once

#include "Math.h"
#include <cstddef>

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
     * @brief Lanczos kernel with a=1
     * @param x bounded input in the [-1, 1] range
     */
    template <typename T>
    static T lanczos1(T x) noexcept
    {
        const auto pix = x * pi<T>;

        const auto sinx = sin9(pix);

        return (sinx * sinx) / (pix * pix);
    }

    /**
     * @brief Lanczos kernel with a=2
     * @param x bounded input in [-2, 2] range
     */
    template <typename T>
    static T lanczos2(T x) noexcept
    {
        static const T a = 2.0;
        static const T ainv = 1.0 / a;

        const auto pix = x * pi<T>;

        const auto sina = sin(pix * ainv);
        const auto sinx = sin(pix);

        return (a * sinx * sina) / (pix * pix);
    }

    /**
     * @brief Lanczos kernel with a=3
     * @param x bounded input in [-3, 3] range
     */
    template <typename T>
    static T lanczos3(T x) noexcept
    {
        static const T a = 3.0;
        static const T ainv = 1.0 / a;

        if (abs(x) < 1e-12) return T(1.0);
        if (abs(x) >= a) return T(0.0);

        const auto pix = x * pi<T>;

        const auto sina = sin9(pix * ainv);
        const auto sin = -chebyshev3(sina);

        return (a * sin * sina) / (pix * pix);
    }

    template <typename T>
    struct ResamplerState
    {
        static const int a = 3; // kernel radius
        static const int kernel_size = a*2 + 1;
        T delay[kernel_size] = { 0.0 };
        
        int base = 0.0;
        double offset = 0.0;
    };

    template <typename T1, typename T2>
    static void resample(
        T1* in_buffer, int in_samples, double in_sr, 
        T2* out_buffer, int& out_samples, double out_sr,
        ResamplerState<T1>& state)
    {
        double ratio = in_sr / out_sr;
        double x_in_offset;

        for (int i = 0; (static_cast<double>(i) * ratio) <= static_cast<double>(in_samples - 1); i++)
        {
            double norm = 0.0;

            out_buffer[i] = T2(0); // reset sample before proceeding

            auto x_in_center = static_cast<double>(i) * ratio + state.offset - static_cast<double>(state.a);

            for (int j = -state.a; j <= state.a; j++)
            {
                // input buffer x coordinate:
                auto x_in = x_in_center + static_cast<double>(j);
                // rounded:
                int x_in_i = std::floor(x_in);
                // fractional offset:
                x_in_offset = x_in - static_cast<double>(x_in_i);
                
                // lanczos kernel x coordinate:
                auto x_lan = static_cast<double>(j) - x_in_offset;
                // lanczos kernel:
                auto y_lan = lanczos3(x_lan);

                // add kernel value
                norm += y_lan;
                
                T1 sample;                
                // if x_in is negative, grab a sample from state's delay line
                if (x_in_i < 0) 
                    sample = state.delay[x_in_i + state.kernel_size];
                // else grab it from input buffer
                else if (x_in_i < in_samples) 
                    sample = in_buffer[x_in_i];
                // shouldn't happen
                else continue;

                out_buffer[i] += T2(sample * y_lan);
            }
            
            out_buffer[i] /= T2(norm);
            out_samples = i + 1;
        }

        state.offset = frac(x_in_offset + ratio);

        for (int i = 0; i < state.kernel_size; i++)
        {
            state.delay[i] = in_buffer[in_samples - state.kernel_size + i];
        }
    }

}