#pragma once

namespace anim
{
    template <typename T>
    class ValueEvaluator
    {
    public:
        /**
         * @brief This function returns the result of linearly interpolating the start and end values, with
         * <code>fraction</code> representing the proportion between the start and end values. The
         * calculation is a simple parametric calculation: <code>result = start_value + fraction * (end_value - start_value)</code>
         *
         * @param fraction The fraction from the starting to the ending values.
         * @param start_value The start value.
         * @param end_value The end value.
         * @return T A linear interpolation between the start and end values, given the <code>fraction</code> parameter.
         */
        virtual T Evaluate(float fraction, const T& start_value, const T& end_value) const {
            return start_value + fraction * (end_value - start_value);
        }
    };
}