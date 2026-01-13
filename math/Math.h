#pragma once

#include <cmath>
#include <cstdlib>
#include <numbers>

namespace Ath::Math
{
    // ============================================================
    // BASIC FUNCTIONS
    // ============================================================

    /**
     * @brief Sign function with zero mapped to +1.
     *
     * Returns:
     *  - +1 if val >= 0
     *  - -1 if val < 0
     *
     * Implemented branchlessly via boolean-to-integer conversion.
     */
    template <typename T> 
    static inline T sign(T val)
    {   auto cond = val >= T(0);
        return T(cond) - T(!cond);
    }

    /**
     * @brief Strict sign function with zero mapped to 0.
     *
     * Returns:
     *  +1 if val > 0
     *
     *  0 if val == 0
     *
     *  -1 if val < 0
     *
     * Implemented without branches using two comparisons.
     */
    template <typename T> 
    static inline T sign0(T val)
    {   auto condP = val > T(0);
        auto condN = val < T(0);
        return T(condP) - T(condN);
    }

    /**
     * @brief Absolute value.
     *
     * Branch-based implementation.
     */
    template <typename T>
    static inline T abs (T x)
    {
        return (x >= 0) ? x : -x;
    }

    /**
     * @brief Fast truncation toward zero.
     *
     * Equivalent to std::trunc for floating-point types,
     * implemented via integer cast.
     */
    template <typename T>
    static inline T trunc (T x)
    {
        return static_cast<int>(x);
    }

    /**
     * @brief Fractional part.
     *
     * Defined as x - trunc(x).
     * For negative values, this follows truncation semantics
     * (i.e. fractional part may be negative).
     */
    template <typename T>
    static inline T frac (T x)
    {
        return x - static_cast<int>(x);
    }

    template <typename T>
    static inline T max(T a, T b)
    {
        return a > b ? a : b;
    }

    template <typename T>
    static inline T min(T a, T b)
    {
        return a < b ? a : b;
    }

    /**
     * @brief Clamp x to the range [a, b].
     *
     * Implemented using min/max composition.
     */
    template <typename T>
    static inline T clamp(T x, T a, T b)
    {
        return min(max(x, a), b);
    }

    // ============================================================
    // POWERS
    // ============================================================

    // Returns t^2, inline.
    template<typename T>
    static inline T quad(T t) { return t * t; }

    // Returns t^3, inline.
    template<typename T>
    static inline T cube(T t) { return t * t * t; }

    // Returns t^4, inline.
    template<typename T>
    static inline T quart(T t) { return t * t * t * t; }

    // Returns t^5, inline.
    template<typename T>
    static inline T quint(T t) { return t * t * t * t * t; }

    /**
     * @brief Inverted quadratic easing.
     *
     * Computes: 1 - (1 - t)^2
     */
    template<typename T>
    static inline T quadInv(T t)
    {
        T x1 = T(1.0f) - t;
        return T(1.0f) - quad(x1);
    }

    /**
     * @brief Inverted cubic easing.
     *
     * Computes: 1 - (1 - t)^3
     */
    template<typename T>
    static inline T cubeInv(T t)
    {
        T x1 = T(1.0f) - t;
        return T(1.0f) - cube(x1);
    }

    /**
     * @brief Inverted quartic easing.
     *
     * Computes: 1 - (1 - t)^4
     */
    template<typename T>
    static inline T quartInv(T t)
    {
        T x1 = T(1.0f) - t;
        return T(1.0f) - quart(x1);
    }

    /**
     * @brief Inverted quintic easing.
     *
     * Computes: 1 - (1 - t)^5
     */
    template<typename T>
    static inline T quintInv(T t)
    {
        T x1 = T(1.0f) - t;
        return T(1.0f) - quint(x1);
    }

    /**
     * @brief Integer power with runtime exponent.
     *
     * Uses exponentiation by squaring.
     * Supports negative exponents via reciprocal.
     *
     * constexpr allows compile-time evaluation when inputs are constant.
     */
    template <typename T>
    constexpr T ipow(T a, int n) 
    {
        if (n == 0) return T(1.0);
        if (n < 0)  return T(1.0) / ipow(a, -n);

        if (n % 2 == 0) 
        {
            T half = ipow(a, n / 2);
            return half * half;
        }

        return a * ipow(a, n - 1);
    }

    /**
     * @brief Integer power with compile-time exponent.
     *
     * Fully unrolled at compile time when possible.
     */
    template <typename T, int N>
    constexpr T ipow(T a) 
    {
        if constexpr (N == 0) return T(1.0);
        if constexpr (N < 0)  return T(1.0) / ipow(a, -N);

        if constexpr (N % 2 == 0) 
        {
            T half = ipow(a, N / 2);
            return half * half;
        }

        return a * ipow(a, N - 1);
    }

    /**
     * @brief Inverted integer-power easing.
     *
     * Computes: 1 - (1 - t)^N
     */
    template<typename T, int N>
    static inline T ipowInv(T t)
    {
        T x1 = T(1.0f) - t;
        return T(1.0f) - ipow<N>(x1);
    }

    // ============================================================
    // INTERPOLATION
    // ============================================================

    /**
     * @brief Linear interpolation
     *
     * Interpolates linearly between a and b.
     */
    template <typename T>
    static inline T lerp(T a, T b, T t)
    {
        return a + (b - a) * t;
    }

    /**
     * @brief Logarithmic interpolation in base 2.
     *
     * Interpolates exponentially between a and b.
     */
    template <typename T>
    T logerp2 (T a, T b, T x)
    {
        const T log = std::log2(b / a);
        return a * std::pow(T(2), log * x);
    }

    /**
     * @brief Logarithmic interpolation in base 10.
     *
     * Interpolates exponentially between a and b.
     */
    template <typename T>
    T logerp10 (T a, T b, T x)
    {
        const T log = std::log10(b / a);
        return a * std::pow(T(10), log * x);
    }

    // ============================================================
    // TRIGONOMETRY
    // ============================================================

    // Mathematical constants
    template <typename T> static constexpr T pi = std::numbers::pi_v<T>;
    template <typename T> static constexpr T tau = pi<T> * T(2.0);

    template <typename T> static constexpr T invPi = T(1.0) / pi<T>;
    template <typename T> static constexpr T invTau = T(1.0) / tau<T>;

    static constexpr float fpi = pi<float>;
    static constexpr float ftau = pi<float> * 2.0f;

    static constexpr float finvPi = invPi<float>;
    static constexpr float finvTau = invTau<float>;

    /**
     * @brief Pade approximation of sin for [-pi, pi] input x.
     */
    template <typename T>
    static T sin (T x) noexcept
    {
        T p1 = 24019.5385697205;
        T p3 = -3423.34761261891;
        T p5 = 110.14197630042;

        T q0 = 24019.5385697205;
        T q2 = 579.908815667847;
        T q4 = 6.63062416405668;
        T q6 = 0.0383120256901944;

        auto x2 = x * x;
        auto numerator = x * (p1 + x2 * (p3 + x2 * (p5 - x2)));
        auto denominator = q0 + x2 * (q2 + x2 * (q4 + x2 * q6));
        return numerator / denominator;
    }

    /**
     * @brief Rational approximation of sin(2pi * x) for [-0.5, 0.5] input x.
     */
    template <typename T>
    static T sin2pi (T x) noexcept
    {
        auto x2 = x * x;
        auto numerator = x * (0.390378103540111 + x2 * (-2.19650161517261 + x2 * (2.78992885186622 - x2)));
        auto denominator = 0.0621306048532483 + x2 * (0.0592189045039231 + x2 * (0.0267309754425848 + x2 * 0.00609754826845813));
        return numerator / denominator;
    }

    /**
     * @brief Rational approximation of sin(2pi * x) for [0.0, 1.0] input x.
     */
    template <typename T>
    static T sin2pi01 (T x) noexcept
    {
        x -= 0.5;
        auto x2 = x * x;
        auto numerator = x * (0.390378103540111 + x2 * (-2.19650161517261 + x2 * (2.78992885186622 - x2)));
        auto denominator = 0.0621306048532483 + x2 * (0.0592189045039231 + x2 * (0.0267309754425848 + x2 * 0.00609754826845813));
        return -numerator / denominator;
    }

    template <typename T>
    static T sin9 (T x) noexcept
    {
        T p1 = 3588316.43293775385245680809;
        T p3 = -528360.77420690737199038267;
        T p5 = 18957.81813070576390600763;
        T p7 = -238.98435213716663838568;

        T q0 = 3588316.43293829867616295815;
        T q2 = 69691.96461030155478511006;
        T q4 = 670.50863659986623588338;
        T q6 = 3.96825138090984630423;
        T q8 = 0.01314065665343438528;

        auto x2 = x * x;
        auto numerator = x * (p1 + x2 * (p3 + x2 * (p5 + x2 * (p7 + x2))));
        auto denominator = q0 + x2 * (q2 + x2 * (q4 + x2 * (q6 + x2 * q8)));
        return numerator / denominator;
    }

    template <typename T>
    static T sin2pi9 (T x) noexcept
    {
        T p1 = 1.47724432246904480159;
        T p3 = -8.58720584584061974454;
        T p5 = 12.16380129137498933289;
        T p7 = -6.05354827006795037647;

        T q0 = 0.23511073607542215536;
        T q2 = 0.18027037928061467875;
        T q4 = 0.06847091023266492493;
        T q6 = 3.96825138090984630423;
        T q8 = 0.00209141397521427812;

        auto x2 = x * x;
        auto numerator = x * (p1 + x2 * (p3 + x2 * (p5 + x2 * (p7 + x2))));
        auto denominator = q0 + x2 * (q2 + x2 * (q4 + x2 * (q6 + x2 * q8)));
        return numerator / denominator;
    }

    template <typename T>
    static T sin2pi9_01 (T x) noexcept
    {
        T p1 = 1.47724432246904480159;
        T p3 = -8.58720584584061974454;
        T p5 = 12.16380129137498933289;
        T p7 = -6.05354827006795037647;

        T q0 = 0.23511073607542215536;
        T q2 = 0.18027037928061467875;
        T q4 = 0.06847091023266492493;
        T q6 = 3.96825138090984630423;
        T q8 = 0.00209141397521427812;

        x -= 0.5;
        auto x2 = x * x;
        auto numerator = x * (p1 + x2 * (p3 + x2 * (p5 + x2 * (p7 + x2))));
        auto denominator = q0 + x2 * (q2 + x2 * (q4 + x2 * (q6 + x2 * q8)));
        return -numerator / denominator;
    }

    /**
     * @brief Pade approximation of tan on [-pi/2, pi] range.
     */
    template <typename T>
    static inline T fastTan(T x) noexcept
    {
        const T x2 = x * x;

        const T a = -135135.0;
        const T b = 17325.0;
        const T c = -378.0;
        const T d = 62370.0;
        const T e = -3150.0;
        const T f = 28.0;

        const T numerator = x * (a + x2 * (b + x2 * (c + x2)));
        const T denominator = a + x2 * (d + x2 * (e + f * x2));
        return numerator / denominator;
    }

    /**
     * @brief Fold a normalized phase into [-0.5, 0.5].
     */
    template <typename T>
    static inline T foldArgument(T x)
    {
        const T half = 0.5;
        x = frac(x + half) - half;

        return max(min(x, half - x), -half - x);
    }

    /**
     * @brief Parabolic sine approximation for sin(2πx).
     *
     * Approximates sin(2πx) with quadratic curves.
     */
    template <typename T>
    static inline T sin2piParabola(T x) noexcept
    {
        const T x1 = foldArgument(x);
        const T x2 = x1 * x1;
        const T xabs = abs(x1);

        return (xabs * 4.0 - x2 * 8.0) * 2.0 * sign(x1);
    }

    /**
        * @brief Sin(2πx). Expects 0...1 input.
        *
        * 5th-order odd polynomial approximation.
        * Max measured absolute error: ~6.78e-05 (~ -80 dB).
        * More accurate than JUCE's Pade approximation.
        */
    template <typename T>
    static inline T sin2pi5(T x) noexcept
    {
        const T x1 = foldArgument(x);
        const T x2 = x1 * x1;

        const T a = 6.2812800766220821491468958126456729;
        const T b = -41.0952426871208970211323332525800187;
        const T c = 73.5855147347551640956688672796423323;

        return x1 * (a + x2 * (b + c * x2));
    }

    /**
        * @brief Sin(2πx). Expects 0...1 input.
        *
        * 7th-order odd polynomial.
        * Max measured abs error: ~8.34e-07 (~ -121 dB).
        */
    template <typename T>
    static inline T sin2pi7(T x) noexcept
    {
        const auto x1 = foldArgument(x);
        const auto x2 = x1 * x1;

        const T a = 6.28316404430247135671540270030948533;
        const T b = -41.3371423711001029236311900250633048;
        const T c = 81.3407688876640676542096535737693472;
        const T d = -70.9934332720751750562132689396061123;

        return x1 * (a + x2 * (b + x2 * (c + d * x2)));
    }

    /**
     * @brief Chebyshev polynomial T_2(x).
     *
     * Explicit form: 2x^2 − 1
     */
    template <typename T>
    static inline T chebyshev2(T x) noexcept
    {
        return x * x * 2.0 - 1.0;
    }

    /**
     * @brief Chebyshev polynomial T_3(x).
     *
     * Explicit form: 4x^3 − 3x
     */
    template <typename T>
    static inline T chebyshev3(T x) noexcept
    {
        return x * x * x * 4.0 - x * 3.0;
    }

    /**
     * @brief Chebyshev polynomial T_5(x).
     */
    template <typename T>
    static inline T chebyshev5(T x) noexcept
    {
        const T x2 = x * x;
        const T x3 = x2 * x;
        const T x5 = x3 * x2;

        return x5 * 16.0 - x3 * 20 + x * 5;
    }

    /**
     * @brief Chebyshev polynomial T_7(x).
     */
    template <typename T>
    static inline T chebyshev7(T x) noexcept
    {
        const T x2 = x * x;
        const T x3 = x2 * x;
        const T x5 = x3 * x2;
        const T x7 = x5 * x2;

        return x7 * 64.0 - x5 * 112.0 + x3 * 56.0 - x * 7.0;
    }

    /**
     * @brief Chebyshev recurrence relation.
     *
     * T_{n + 1}(x) = 2xT_n(x) − T_{n - 1}(x)
     */
    template <typename T>
    static inline T chebyshev_nplus1(T x, T t_n, T t_nminus1) noexcept
    {
        return x * t_n * T(2.0) - t_nminus1;
    }

    // ============================================================
    // NOTE / FREQUENCY
    // ============================================================

    static constexpr int C1_MIDI_NOTE_NUMBER = 24;
    static constexpr int C2_MIDI_NOTE_NUMBER = C1_MIDI_NOTE_NUMBER + 12;
    static constexpr int A4_MIDI_NOTE_NUMBER = 69;

    /**
     * @brief MIDI note number to frequency.
     */
    template <typename T>
    T noteToFrequency (int p, T referenceFrequency = T(440))
    {
        return referenceFrequency
             * std::pow(T(2), T(p - A4_MIDI_NOTE_NUMBER) / T(12));
    }

    /**
     * @brief MIDI note number to frequency.
     */
    template <typename T>
    T noteToFrequency (T p, T referenceFrequency = T(440))
    {
        return referenceFrequency
             * std::pow(T(2), (p - T(A4_MIDI_NOTE_NUMBER)) / T(12));
    }

    /**
     * @brief Frequency to MIDI note number.
     */
    template <typename T>
    T frequencyToNote (T freq, T referenceFrequency = T(440))
    {
        return T(A4_MIDI_NOTE_NUMBER)
             + T(12) * std::log2(freq / referenceFrequency);
    }

    template <typename T>
    T semitonesToFrequencyRatio (T semitones)
    {
        return std::pow(T(2), semitones / T(12));
    }

    // ============================================================
    // dB CONSTANTS
    // ============================================================

    /**
     * @brief Integer dB to linear amplitude.
     *
     * Uses base = 10^(1/20), raised via integer power.
     */
    constexpr float dB(int dB)
    {
        constexpr float base = 1.1220184543019633; // 10^(1/20)
        return ipow(base, dB);
    }

    static constexpr float DB_MINUS1  = 0.891250938f;
    static constexpr float DB_MINUS2  = 0.794328235f;
    static constexpr float DB_MINUS3  = 0.707945784f;
    static constexpr float DB_MINUS6  = 0.501187234f;
    static constexpr float DB_MINUS9  = DB_MINUS3  * DB_MINUS6;
    static constexpr float DB_MINUS12 = DB_MINUS6  * DB_MINUS6;
    static constexpr float DB_MINUS18 = DB_MINUS12 * DB_MINUS6;
    static constexpr float DB_MINUS24 = DB_MINUS18 * DB_MINUS6;
    static constexpr float DB_MINUS30 = DB_MINUS24 * DB_MINUS6;
    static constexpr float DB_MINUS36 = DB_MINUS30 * DB_MINUS6;
    static constexpr float DB_MINUS42 = DB_MINUS36 * DB_MINUS6;
    static constexpr float DB_MINUS48 = DB_MINUS42 * DB_MINUS6;
    static constexpr float DB_MINUS54 = DB_MINUS48 * DB_MINUS6;
    static constexpr float DB_MINUS60 = DB_MINUS54 * DB_MINUS6;
    static constexpr float DB_MINUS66 = DB_MINUS60 * DB_MINUS6;
    static constexpr float DB_MINUS72 = DB_MINUS66 * DB_MINUS6;
    static constexpr float DB_MINUS78 = DB_MINUS72 * DB_MINUS6;
    static constexpr float DB_MINUS84 = DB_MINUS78 * DB_MINUS6;

    static constexpr float DB_MINUS50 = 0.003162278f;

    static constexpr float DB_PLUS1 = 1.0f / DB_MINUS1;
    static constexpr float DB_PLUS2 = 1.0f / DB_MINUS2;
    static constexpr float DB_PLUS3 = 1.0f / DB_MINUS3;

    // ============================================================
    // AMPLITUDE / dB CONVERSIONS
    // ============================================================

    template <typename T>
    T amplitudeToDecibels (T gain)
    {
        return T(20) * std::log10(gain);
    }

    template <typename T>
    T decibelsToAmplitude (T db)
    {
        return std::pow(T(10), db / T(20));
    }

    template <typename T>
    T amplitudeRatioToDecibels (T ratio)
    {
        return T(10) * std::log10(ratio);
    }

    template <typename T>
    T amplitudeRatioToDecibels (T v1, T v2)
    {
        return T(10) * std::log10(v1 / v2);
    }

    template <typename T>
    T decibelsToAmplitudeRatio (T db)
    {
        return std::pow(T(10), db / T(10));
    }

    /**
     * @brief Linear slider mapped to logarithmic amplitude.
     *
     * x ∈ [0, 1], interpolated between dB_at0 and 0 dB,
     * then converted to linear amplitude.
     */
    template <typename T>
    T linearVolumeToLog (T x, T dB_at0)
    {
        const T dB = std::lerp(dB_at0, T(0), x);
        return decibelsToAmplitude(dB);
    }
    
}
