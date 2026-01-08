#include "Random.h"

namespace Ath::Math::Random
{
    float RandomNumberGeneratorBase::getFloat() { return std::abs(float (getInt()) * 4.6566129e-10f); }
    float RandomNumberGeneratorBase::getFloatBipolar() { return getFloat() * 2.0f - 1.0f; }
    float RandomNumberGeneratorBase::getFloatNormal() { return standardNormalDistributionCDF.evaluate (getFloatBipolar()); }

    void LinearCongruentialGenerator::setSeed(int newSeed) { seed = newSeed; }
    int LinearCongruentialGenerator::getInt() { seed = seed * a + c; return seed;}

    void MersenneTwister::setSeed (int newSeed) { rng.seed (newSeed); }
    int MersenneTwister::getInt() { return rng(); }
}
