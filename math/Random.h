#pragma once

#include <cstdint>
#include <random>

#include "Polynomial.h"

namespace Ath::Math::Random
{
    class RandomNumberGeneratorBase
    {
    public:

        // Polynomial mapping of -1...+1 range to cumulative distribution function of standard normal distribution
        Polynomial<float, 14> standardNormalDistributionCDF =
        {
            {
                3.33595790e+01, -8.17321894e-01, -9.24903896e+01,  2.18475291e+00,
                9.76784663e+01, -2.20902174e+00, -4.86393674e+01,  1.05514441e+00,
                1.15804712e+01, -2.52570282e-01, -1.05575550e+00,  3.16332822e-02,
                3.70656768e-01, -4.61283192e-03
            }
        };

        virtual int getInt() = 0;

        virtual long long getLong() = 0;

        float getFloat();

        double getDouble();

        float getFloatBipolar();

        float getFloatNormal();
    };

    /**
     * @brief Linear congruential generator.
     *
     * Default parameters from Numerical Recipes, 
     * Chapter 7.1, §An Even Quicker Generator
     * With default parameters, periodicity is 2^32
     */
    class LinearCongruentialGenerator : public RandomNumberGeneratorBase
    {
        uint32_t seed = 0;
        uint32_t seed1 = 0;

    public:
        uint32_t a = 1664525;
        uint32_t c = 1013904223;
    
        void setSeed(int newSeed);
        int getInt() override;
        long long getLong() override;
    };

    /**
     * @brief 64-bit linear congruential generator.
     *
     * https://nuclear.llnl.gov/CNP/rng/rngman/node4.html
     * With default parameters, periodicity is 2^64
     */
    class LinearCongruentialGenerator64 : public RandomNumberGeneratorBase
    {
        uint64_t seed = 0;

    public:
        uint64_t a = 2862933555777941757ULL;
        uint64_t c = 3037000493ULL;

        void setSeed(uint64_t newSeed);
        int getInt() override;
        long long getLong() override;
    };

    using Lcg32 = LinearCongruentialGenerator;
    using Lcg64 = LinearCongruentialGenerator64;

    /**
     * @brief MT19937-64 Mersenne Twister with periodicity of 2^19937 − 1
     */
    class MersenneTwister : public RandomNumberGeneratorBase
    {
        std::mt19937_64 rng = std::mt19937_64();

    public:
        void setSeed (int newSeed);
        int getInt() override;
        long long getLong() override;
    };
}
