#pragma once
#include "cc_value_evaluator.h"

namespace anim
{
    template <typename T>
    class Keyframe
    {
    public:
        Keyframe(float fraction, T value) : fraction_(fraction), value_(value) {}
        float Fraction() const { return fraction_; }
        T Value() const { return value_; }

    private:
        float fraction_;
        T value_;
    };

    template <typename T>
    class KeyframeSet
    {
    public:
    private:
        Keyframe<T> first_keyframe_;
        Keyframe<T> last_keyframe_;
        std::vector<Keyframe<T>> keyframes_;
        ValueEvaluator<T> evaluator_;
        int num_keyframes_;
    };
}