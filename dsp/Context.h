#pragma once

namespace Ath::Dsp
{
    struct Context
    {
        float SR;
        float T;

        int maxSamplesPerBlock = 1;

        Context();

        Context(float sampleRate);

        Context(float sampleRate, int samplesPerBlock);
    };
}
