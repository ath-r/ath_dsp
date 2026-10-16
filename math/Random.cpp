#include "Random.h"

#include <cmath>

namespace Ath::Math::Random
{
    namespace
    {
        long long longFromTwoInts (RandomNumberGeneratorBase& rng)
        {
            const auto hi = static_cast<uint32_t> (rng.getInt());
            const auto lo = static_cast<uint32_t> (rng.getInt());
            return static_cast<long long> ((static_cast<uint64_t> (hi) << 32) | lo);
        }
    }

    float RandomNumberGeneratorBase::getFloat() { return std::abs(float (getInt()) * 4.6566129e-10f); }
    double RandomNumberGeneratorBase::getDouble()
    {
        // Top 53 bits. The low bits of an LCG have a much shorter period.
        const auto bits = static_cast<uint64_t> (getLong()) >> 11;
        return std::ldexp (static_cast<double> (bits), -53);
    }
    float RandomNumberGeneratorBase::getFloatBipolar() { return getFloat() * 2.0f - 1.0f; }
    float RandomNumberGeneratorBase::getFloatNormal() { return standardNormalDistributionCDF.evaluate (getFloatBipolar()); }

    void LinearCongruentialGenerator::setSeed(int newSeed) { seed = newSeed; }
    int LinearCongruentialGenerator::getInt() { seed = seed * a + c; return static_cast<int>(seed);}
    long long LinearCongruentialGenerator::getLong() { return longFromTwoInts (*this); }

    void LinearCongruentialGenerator64::setSeed(uint64_t newSeed) { seed = newSeed; }
    int LinearCongruentialGenerator64::getInt() { seed = seed * a + c; return static_cast<int>(seed >> 32); }
    long long LinearCongruentialGenerator64::getLong() { seed = seed * a + c; return static_cast<long long> (seed); }

    void MersenneTwister::setSeed (int newSeed) { rng.seed (static_cast<uint64_t> (newSeed)); }
    int MersenneTwister::getInt() { return static_cast<int> (rng() >> 32); }
    long long MersenneTwister::getLong() { return static_cast<long long> (rng()); }
}
