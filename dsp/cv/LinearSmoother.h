#pragma once

#include "../Context.h"
#include "../../math/Math.h"

namespace Ath::Dsp::Cv
{
    template <typename T>
    class ConstantRateLinearSmoother
    {
    protected:

        Context c = Context(48000.0f);
        T targetValue = 0.0;
        T currentValue = 0.0;

        T time = 0.0;
        T delta = 0.0;

    public:
        void reset()
        {
            currentValue = 0.0f;
            targetValue = 0.0f;
        }

        void setContext(const Context context)
        {
            c = context;
            setTime(time);
        }

        virtual void setTime(T newTime)
        {
            time = newTime;

            delta = c.T / time;
        }

        virtual inline void setTargetValue(T value)
        {
            targetValue = value;
        }

        inline T last()
        {
            return currentValue;
        }

        virtual T process()
        {
            auto diff = targetValue - currentValue;
            diff = Math::clamp(diff, -delta, delta);
            currentValue += diff;

            return currentValue;
        };

        virtual inline T process(const T value)
        {
            setTargetValue(value);
            return process();
        }
    };

    template <typename T>
    class ConstantTimeLinearSmoother : public ConstantRateLinearSmoother<T>
    {
    private:
        using Base = ConstantRateLinearSmoother<T>;
        
        void calculateDelta()
        {
            auto diff = Math::abs(Base::targetValue - Base::currentValue);
            Base::delta = Base::c.T / Base::time * diff;
        }

    public:
        void setTime(T newTime) override
        {
            Base::time = newTime;
            calculateDelta();
        }

        void setTargetValue(T value) override
        {
            Base::targetValue = value;
            calculateDelta();
        }
    };
}
