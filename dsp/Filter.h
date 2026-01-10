#pragma once

#include <array>
#include <cmath>
#include <type_traits>

#include "Context.h"
#include "../math/Complex.h"
#include "../math/Math.h"

namespace Ath::Dsp::Filter
{
    /**
     * @brief Convert normalized frequency to filter coefficient g for simple one-pole filters.
     *
     * @param freq Normalized frequency (0..0.5)
     * @return Computed filter coefficient g
     */
    template <typename T>
    static inline T normFrequencyToG(T freq)
    {
        const T g = freq * Math::fpi;
        return g / (g + T(1.0f));
    }

    /**
     * @brief Convert frequency in Hz to filter coefficient g.
     *
     * @param freq Frequency in Hz
     * @param sampleT Sample period (1/sampleRate)
     * @return Filter coefficient g
     */
    template <typename T>
    static inline T frequencyToG(T freq, T sampleT)
    {
        return normFrequencyToG(freq * sampleT);
    }

    /**
     * @brief Convert smoothing time to filter coefficient g.
     *
     * Assumes a one-pole filter; shorter times produce faster responses.
     *
     * @param time Time constant in seconds
     * @param sampleT Sample period (1/sampleRate)
     * @return Filter coefficient g
     */
    template <typename T>
    static inline T timeToG(T time, T sampleT)
    {
        const T freq = T(0.5f) * sampleT / time;
        return normFrequencyToG(freq);
    }

    /**
     * @brief Map frequency to a complex "s" value in the Laplace domain.
     *
     * @param frequency Frequency in Hz
     * @return Complex number representing s = j*omega
     */
    template <typename T>
    static Math::complex<T> f2s(T frequency)
    {
        return { 0.0f, frequency * Math::ftau };
    }

    /**
     * @brief Bilinear transform: converts an analog s-domain value to the z-domain.
     *
     * @param s Analog s-domain complex value
     * @param sr Sample rate
     * @return Bilinear-transformed z-domain complex value
     */
    template <typename T>
    static Math::complex<T> bilinear(Math::complex<T> s, T sr)
    {
        Math::complex<T> k = T(sr * 2.0);
        return (k + s) / (k - s);
    }

    /**
     * @brief One-pole low-pass transfer function in s-domain.
     */
    template<typename T>
    static Math::complex<T> transferLP1(Math::complex<T> wc, Math::complex<T> s)
    {
        return wc / (wc + s);
    }

    /**
     * @brief One-pole high-pass transfer function in s-domain.
     */
    template<typename T>
    static Math::complex<T> transferHP1(Math::complex<T> wc, Math::complex<T> s)
    {
        return s / (wc + s);
    }

    // ============================================================
    // Naive one-pole filters
    // ============================================================
    namespace Naive
    {
        /**
         * @brief Process one sample through a simple one-pole low-pass filter.
         *
         * @param x Input sample
         * @param y Filter state (previous output)
         * @param g Filter coefficient
         * @return Filtered output
         */
        template <typename T>
        static inline T processLP(T x, T& y, T g)
        {
            y += (x - y) * g;
            return y;
        }

        /**
         * @brief Simple one-pole low-pass filter class
         */
        template <typename T>
        class LowPass1
        {
            Context c;
            T g = 0.0f;      // per-sample coefficient
            T y = 0.0f;      // current filter state
            
            T frequency = 100.0f;

        public:
            void reset() { y = 0; }

            void setContext(const Context context)
            {
                c = context;
                setCutoffFrequency(frequency);
            }

            void setCutoffFrequency(T freq)
            {
                frequency = freq;
                g = frequencyToG(freq, T(c.T));
            }

            virtual inline T process(T x)
            {
                y = processLP(x, y, g);
                return y;
            }

            inline T last() { return y; }
        };
    }

    // ============================================================
    // Transposed Processing Topology (TPT) filters
    // ============================================================
    namespace TPT 
    {
        template <typename T>
        static inline T processLP(T x, T& z1, T G)
        {
            const T v = (x - z1) * G;
            const T y = v + z1;
            z1 = v + y; // update integrator state
            return y;
        }

        template <typename T>
        static inline T processHP(T x, T& z1, T G)
        {
            return x - processLP(x, z1, G);
        }

        template <typename T>
        static inline T processHP(T x, T& z1, T G, T g2)
        {
            const T y = (x - z1) * G;
            z1 = z1 + y * g2;
            return y;
        }

        /**
         * @brief One-pole low-pass filter using TPT structure
         */
        template <typename T>
        class LowPass1
        {
        private:
            Context c;
            T G = 0.0f;
            T z1 = 0.0f;
            T y = 0.0f;

            T frequency = 100.f;

        public:
            void reset() { z1 = 0; }

            void setContext(const Context context)
            {
                c = context;
                setCutoffFrequency(frequency);
            }

            void setCutoffFrequency(T freq)
            {
                frequency = freq;
                G = frequencyToG(freq, T(c.T));
            }

            Math::complex<T> getTransfer(T freq)
            {
                Math::complex<T> wc = { frequency * Math::ftau, 0.0f };
                Math::complex<T> s  = { 0.0f, freq * Math::ftau };
                return transferLP1(wc, s);
            }

            virtual inline T process(T x)
            {
                y = processLP(x, z1, G);
                return y;
            }

            inline T last() { return y; }
        };

        /**
         * @brief One-pole high-pass filter using TPT structure
         */
        template <typename T>
        class HighPass1
        {
            Context c;
            T G = 0.0f;
            T z1 = 0.0f;
            T y = 0.0f;

            T frequency;

        public:
            void reset() { z1 = 0; }

            void setContext(const Context context)
            {
                c = context;
                setCutoffFrequency(frequency);
            }

            void setCutoffFrequency(T freq)
            {
                frequency = freq;
                G = frequencyToG(freq, T(c.T));
            }

            Math::complex<T> getTransfer(T freq)
            {
                Math::complex<T> wc = { frequency * Math::ftau, 0.0f };
                Math::complex<T> s  = { 0.0f, freq * Math::ftau };
                return transferHP1(wc, s);
            }

            inline T process(T x)
            {
                y = processHP(x, z1, G);
                return y;
            }
        };

        /**
         * @brief State-variable filter using TPT method
         *
         * Produces low-pass, band-pass, and high-pass outputs simultaneously.
         * Resonance parameter R controls the filter's Q factor.
         */
        template <typename T>
        class StateVariableFilter
        {
        private:
            Context c;

            // TPT parameters
            T G  = 0.0;   // integrator gain
            T R  = 1.0;   // resonance
            T g1 = 2.0;   // 2R + G
            T d  = 0.0;   // normalization

            // state variables
            T s1 = 0.0;
            T s2 = 0.0;

            // outputs
            T hp = 0.0;
            T bp = 0.0;
            T lp = 0.0;

            T frequency = 100.0;
            T resonance = 0.0;

            inline void updateCoefficients()
            {
                g1 = T(2.0) * R + G;
                d  = T(1.0) / (T(1.0) + g1 * G);
            }

            inline void processInternal(T x)
            {
                // High-pass
                hp = (x - g1 * s1 - s2) * d;

                // Band-pass
                T v1 = G * hp;
                bp   = v1 + s1;
                s1   = bp + v1;

                // Low-pass
                T v2 = G * bp;
                lp   = v2 + s2;
                s2   = lp + v2;
            }

        public:
            void reset() { s1 = 0; s2 = 0; }

            void setContext(const Context context)
            {
                c = context;
                setCutoffFrequency(frequency);
                setResonance(resonance);
            }

            void setCutoffFrequency(T freq)
            {
                frequency = freq;
                G = frequencyToG(freq, T(c.T));
                updateCoefficients();
            }

            void setResonance(T r)
            {
                resonance = Math::clamp(r, T(0.0), T(1.0));
                static const T max_resonance = T(0.1);
                R = Math::lerp(1.0, max_resonance, resonance);
                updateCoefficients();
            }

            T processHighPass(T x) { processInternal(x); return hp; }
            T processBandPass(T x) { processInternal(x); return bp; }
            T processLowPass(T x)  { processInternal(x); return lp; }

            inline T lastHighPass() const { return hp; }
            inline T lastBandPass() const { return bp; }
            inline T lastLowPass()  const { return lp; }
        };

    } // namespace TPT

    // ============================================================
    // Biquad filters
    // ============================================================
    namespace Biquad
    {
        enum class BiquadTopology
        {
            DirectForm1,
            DirectForm2,
            TransposedDirectForm1,
            TransposedDirectForm2
        };

        template <typename T>
        struct DigitalBiquadCoefficients
        {
            T b0 = 1.0;
            T b1 = 0.0;
            T b2 = 0.0;
            T a0 = 1.0;
            T a1 = 0.0;
            T a2 = 0.0;
        };

        // Analog coefficients have the same structure, semantics differ
        template <typename T>
        struct AnalogBiquadCoefficients 
        {
            T b0 = 1.0;
            T b1 = 0.0;
            T b2 = 0.0;
            T a0 = 1.0;
            T a1 = 0.0;
            T a2 = 0.0;
        };

        /**
         * @brief Compute digital biquad transfer function at a given frequency
         */
        template <typename T>
        static Math::complex<T> transfer(DigitalBiquadCoefficients<T> coeffs, Math::complex<T> s, T sr)
        {
            auto k = sr * 2.0f;
            auto z1 = (k - s) / (k + s);
            auto z2 = z1 * z1;
            return (coeffs.b0 + coeffs.b1 * z1 + coeffs.b2 * z2) / (coeffs.a0 + coeffs.a1 * z1 + coeffs.a2 * z2);
        }

        /**
         * @brief Convert analog biquad coefficients to digital using bilinear transform
         */
        template <typename T>
        static DigitalBiquadCoefficients<T> bilinear(AnalogBiquadCoefficients<T> in, double sr)
        {
            const auto k = sr * 2.0;
            const auto k2 = k * k;

            T a0 = in.a0 + in.a1 * k + in.a2 * k2;
            T b0 = (in.b0 + in.b1 * k + in.b2 * k2) / a0;
            T b1 = (in.b0 * 2.0 - in.b2 * 2.0 * k2) / a0;
            T b2 = (in.b0 - in.b1 * k + in.b2 * k2) / a0;
            T a1 = (in.a0 * 2.0 - in.a2 * 2.0 * k2) / a0;
            T a2 = (in.a0 - in.a1 * k + in.a2 * k2) / a0;

            return { b0, b1, b2, 1.0, a1, a2 };
        }

        /**
         * @brief Generic biquad filter class supporting multiple topologies
         */
        template <typename T, BiquadTopology Topology = BiquadTopology::DirectForm1>
        class Biquad
        {
            T y = 0.0;
            struct Empty {};

            // Compile-time selection of state members based on topology
            static const bool isDF1 = Topology == BiquadTopology::DirectForm1;
            static const bool isTDF1 = Topology == BiquadTopology::TransposedDirectForm1;
            static const bool isDF2orTDF2 = Topology == BiquadTopology::DirectForm2 || Topology == BiquadTopology::TransposedDirectForm2;

            using DF1State = std::conditional_t<isDF1, T, Empty>;
            DF1State x1, x2, y1, y2;

            using TDF1State = std::conditional_t<isTDF1, T, Empty>;
            TDF1State s0, s1, s2, s3;

            using DF2state = std::conditional_t<isDF2orTDF2, T, Empty>;
            DF2state v1, v2;

        public:
            DigitalBiquadCoefficients<T> coeffs;

            Biquad() { reset(); }

            inline void setCoefficients(DigitalBiquadCoefficients<T> newCoeffs) { coeffs = newCoeffs; }

            inline T process(T x)
            {
                if constexpr(Topology == BiquadTopology::DirectForm1)
                {
                    y = coeffs.b0 * x + coeffs.b1 * x1 + coeffs.b2 * x2 - coeffs.a1 * y1 - coeffs.a2 * y2;
                    x2 = x1; x1 = x; y2 = y1; y1 = y;
                    return y;
                }
                if constexpr(Topology == BiquadTopology::DirectForm2)
                {
                    T w = x - coeffs.a1 * v1 - coeffs.a2 * v2;
                    y = coeffs.b0 * w + coeffs.b1 * v1 + coeffs.b2 * v2;
                    v2 = v1; v1 = w;
                    return y;
                }
                if constexpr(Topology == BiquadTopology::TransposedDirectForm1)
                {
                    y = s0 + s2 + coeffs.b0 * x;
                    s0 = s1 + coeffs.b1 * x;
                    s1 = coeffs.b2 * x;
                    s2 = s3 - coeffs.a1 * y;
                    s3 = -coeffs.a2 * y;
                    return y;
                }
                if constexpr(Topology == BiquadTopology::TransposedDirectForm2)
                {
                    y = coeffs.b0 * x + v1;
                    v1 = coeffs.b1 * x - coeffs.a1 * y + v2;
                    v2 = coeffs.b2 * x - coeffs.a2 * y;
                    return y;
                }
            }

            inline T last() { return y; }

            void reset()
            {
                y = 0.0;
                if constexpr(Topology == BiquadTopology::DirectForm1) { x1=x2=y1=y2=0.0; }
                if constexpr(Topology == BiquadTopology::TransposedDirectForm1) { s0=s1=s2=s3=0.0; }
                if constexpr(isDF2orTDF2) { v1=v2=0.0; }
            }
        };

        /**
         * @brief Cascade of N biquads
         */
        template <typename T, int N, BiquadTopology Topology = BiquadTopology::TransposedDirectForm2>
        class BiquadCascade
        {
            T y = 0.0;

        public:
            std::array<Biquad<T, Topology>, N> biquads;

            T process(T x)
            {
                y = x;
                for (auto& biquad : biquads) y = biquad.process(y);
                return y;
            }

            T last() { return y; }
        };
    } // namespace Biquad
} // namespace Ath::Dsp::Filter
