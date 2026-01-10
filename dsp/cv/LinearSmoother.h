#pragma once

#include "../Context.h"
#include "../../math/Math.h"

namespace Ath::Dsp::Cv
{
    /**
     * @brief Linear smoother with a constant maximum rate of change per sample.
     *
     * This smoother advances the output toward a target value by at most
     * a fixed increment (`delta`) each processing step. The rate of change
     * is independent of the distance to the target.
     */
    template <typename T>
    class ConstantRateLinearSmoother
    {
    protected:

        Context c = Context(48000.0f);

        T targetValue = 0.0;
        T currentValue = 0.0;

        /** Smoothing time in seconds */
        T time = 0.0;

        /** Maximum allowed change per processing step */
        T delta = 0.0;

    public:
        /**
         * @brief Reset the smoother state.
         *
         * Both the current and target values are reset to zero.
         */
        void reset()
        {
            currentValue = 0.0f;
            targetValue = 0.0f;
        }

        /**
         * @brief Set a new processing context.
         *
         * Recomputes the internal rate (`delta`) using the current smoothing time.
         *
         * @param context New DSP context
         */
        void setContext(const Context context)
        {
            c = context;
            setTime(time);
        }

        /**
         * @brief Set the smoothing time.
         *
         * Computes a constant per-sample increment such that the value
         * changes at a fixed rate, independent of the remaining distance
         * to the target.
         *
         * @param newTime Smoothing time in seconds
         */
        virtual void setTime(T newTime)
        {
            time = newTime;

            // Maximum change per sample
            delta = c.T / time;
        }

        /**
         * @brief Set a new target value.
         *
         * The value will be approached at a constant rate determined
         * by the current `delta`.
         *
         * @param value New target value
         */
        virtual inline void setTargetValue(T value)
        {
            targetValue = value;
        }

        /**
         * @brief Retrieve the most recent output value.
         *
         * @return Current smoothed value
         */
        inline T last()
        {
            return currentValue;
        }

        /**
         * brief Process one smoothing step.
         *
         * Advances the current value toward the target by clamping
         * the per-sample change to ±delta.
         *
         * @return Updated smoothed value
         */
        virtual T process()
        {
            auto diff = targetValue - currentValue;
            diff = Math::clamp(diff, -delta, delta);
            currentValue += diff;

            return currentValue;
        };

        /**
         * @brief Update the target value and process one step.
         *
         * @param value New target value
         * @return Updated smoothed value
         */
        virtual inline T process(const T value)
        {
            setTargetValue(value);
            return process();
        }
    };

    /**
     * @brief Linear smoother with constant transition time.
     *
     * Unlike ConstantRateLinearSmoother, this variant adjusts the rate
     * dynamically so that the target value is reached in a fixed amount
     * of time, regardless of the distance to the target.
     *
     * This results in linear interpolation over a constant duration.
     */
    template <typename T>
    class ConstantTimeLinearSmoother : public ConstantRateLinearSmoother<T>
    {
    private:
        using Base = ConstantRateLinearSmoother<T>;
        
        /**
         * @brief Recalculate the per-sample increment.
         *
         * The increment is scaled by the distance to the target so that
         * the transition completes in exactly `time` seconds.
         */
        void calculateDelta()
        {
            auto diff = Math::abs(Base::targetValue - Base::currentValue);
            Base::delta = Base::c.T / Base::time * diff;
        }

    public:
        /**
         * @brief Set the smoothing time.
         *
         * Updates the internal rate so the transition duration remains constant.
         *
         * @param newTime Smoothing time in seconds
         */
        void setTime(T newTime) override
        {
            Base::time = newTime;
            calculateDelta();
        }

        /**
         * @brief Set a new target value.
         *
         * Recomputes the rate so the new target is reached in the configured time.
         *
         * @param value New target value
         */
        void setTargetValue(T value) override
        {
            Base::targetValue = value;
            calculateDelta();
        }
    };
}
