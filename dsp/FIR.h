#pragma once

#include <cmath>
#include <cstddef>
#include <vector>
#include <algorithm>

namespace Ath::Dsp::Filter::Fir
{
    /**
     * @brief Computes coefficients for a linear-phase low-pass filter. 
     * @param cutoff Cutoff frequency in hertz.
     * @param duration Kernel duration in seconds.
     * @param sr Sample Rate in hertz.
     * @note Group delay will be duration/2. Kernel size will be sr*duration.
     */
    static std::vector<double> WindowedSincLowpass(double cutoff, double duration, double sr)
    {
        size_t N = static_cast<size_t>(sr * duration);
        if (N % 2 == 0) N -= 1;
        size_t M = N - 1;

        const auto wc = (cutoff / sr) * std::numbers::pi * 2.0;

        std::vector<double> coefficients(N);

        double sum = 0.0;

        for (size_t n = 0; n < N; n++)
        {
            // Sinus Cardinalis:
            const auto x = (static_cast<double>(n) - static_cast<double>(M) * 0.5) * wc;
            const auto sinc = (x == 0) ? 1.0 : std::sin(x) / x;

            // Blackman-Nuttall window::
            constexpr auto a0 = 0.3635819;
            constexpr auto a1 = 0.4891775;
            constexpr auto a2 = 0.1365995;
            constexpr auto a3 = 0.0106411;

            const auto nm = static_cast<double>(n) /  static_cast<double>(M); 
            const auto wx2 = nm * 2.0 * std::numbers::pi;
            const auto wx4 = wx2 * 2.0;
            const auto wx6 = wx2 * 3.0;

            const auto window = a0 - a1 * std::cos(wx2) + a2 * std::cos(wx4) - a3 * std::cos(wx6);

            coefficients[n] = window * sinc;

            sum += coefficients[n];
        }        

        // Normalize impulse response:
        for (auto& c : coefficients)
        {
            c /= sum;
        }

        return coefficients;
    }

    template<typename  T>
    class Filter
    {
        std::vector<T> coefficients;
        std::vector<T> buffer;

        int circularBufferState = 0;

    public:

        void setCoefficients(std::vector<T> newCoefficients)
        {
            coefficients = newCoefficients;

            buffer.resize(coefficients.size() * 2);
            reset();
        }

        void reset()
        {
            std::fill(buffer.begin(), buffer.end(), 0.0);
        }

        T process(T x)
        {
            auto* coefficientsData = coefficients.data();     
            auto n = coefficients.size();       

            auto* bufferData = buffer.data() + circularBufferState + n;
            bufferData[0] = x;
            bufferData[-n] = x;        

            T sum = T(0);

            for( size_t i = 0; i < n; i++)
            {
                sum += coefficientsData[i] * bufferData[-static_cast<std::ptrdiff_t>(i)];                
            }

            circularBufferState = (circularBufferState + 1) % n;

            return sum;
        }
    };
}