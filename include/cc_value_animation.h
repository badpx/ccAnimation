/**
 * @file cc_value_animation.h
 * @brief
 * @version 0.1
 * @date 2022-02-13
 *
 * @copyright Copyright (c) 2022 Kane Dong
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 **/
#pragma once

#include <algorithm>
#include <initializer_list>
#include <vector>
#include "cc_animation.h"
#include "cc_keyframe.h"

namespace anim
{
    template <typename T>
    class ValueAnimation : public Animation
    {
    public:
        enum class RepeatMode
        {
            kRestart,
            kReverse,
        };

        ValueAnimation(const T &start_value, const T &end_value)
            : ValueAnimation({start_value, end_value}) {}

        ValueAnimation(std::initializer_list<T> values) {
            size_t num = values.size();
            // At least 2 values are required
            if (num > 1) {
                int i = 0;
                for (auto it = values.begin(); it != values.end(); i++, it++) {
                    keyframes_.push_back(Keyframe<T>((float)i / (num - 1), *it));
                }
            }
        }

        ValueAnimation(std::initializer_list<Keyframe<T>> keyframes) : keyframes_(keyframes) {
            std::sort(keyframes_.begin(), keyframes_.end());
        }

#if 0
        void SetKeyframe(Keyframe<T>& keyframe) {
            const float progress = keyframe.progress();
            if (progress < 0.0f || progress > 1.0f) {
                // Invalid progress
                return;
            }
            auto result = std::lower_bound(keyframes_.begin(), keyframes_.end(), keyframe);
            if (result == keyframes_.end() || result.first != progress) {
                keyframes_.insert(result, keyframe);
            } else {
                result->set_value(keyframe.Value()); // replaces the previous value
            }

            // TODO: recalculate current interval
        }

        void SetValueAt(float progress, const T& value) {
            SetKeyframe(Keyframe<T>(progress, value));
        }
#endif

        void SetDuration(long duration) override { duration_ = duration; }
        long GetDuration() const override { return duration_; }
        int repeat_count() const { return repeat_count_; }
        void set_repeat_count(int count) { repeat_count_ = count; }
        RepeatMode repeat_mode() const { return repeat_mode_; }
        void set_repeat_mode(RepeatMode mode) { repeat_mode_ = mode; }
        void set_curve_function(EasingCurve::CurveFunction func) { curve_function_ = func; }

    protected:
        void UpdateCurrentTime(long current_time) override {
        }

        void AnimateValue(float progress) {
            // can't interpolate if we don't have at least 2 values
            if (keyframes_.count() < 2) return;

        }

        bool AnimationFrame(long current_time) {

        }

        /**
         * @brief Processes a frame of the animation, adjusting the start time if needed.
         * 
         * @param frame_time  The frame time.
         * @return true  if the animation has ended.
         */
        bool DoAnimationFrame(long frame_time) {

        }

    private:
        std::vector<Keyframe<T>> keyframes_;
        EasingCurve::CurveFunction curve_function_ = EasingCurve::CurveToFunc(EasingCurve::InOutQuad);
        long duration_;
        RepeatMode repeat_mode_ = RepeatMode::kRestart;
        int repeat_count_ = 0;
    };

    template <typename T>
    typename std::enable_if<std::is_unsigned<T>::value, T>::type
    _interpolate(const T &start, const T &end, float progress) {
        return T(start + end * progress - start * progress);
    }

    // the below will apply also end all non-arithmetic types
    template <typename T>
    typename std::enable_if<!std::is_unsigned<T>::value, T>::type
    _interpolate(const T &start, const T &end, float progress) {
        return T(start + (end - start) * progress);
    }

    /**
     * @brief  This function returns the result of linearly interpolating the start and end values, with
     * <code>progress</code> representing the proportion between the start and end values. The
     * calculation is a simple parametric calculation: <code>result = start + (end - start) * progress </code>
     *
     * @tparam T The type of start and end value.
     * @param start The start value.
     * @param end The end value.
     * @param progress The progress from the starting end the ending values.
     * @return T T A linear interpolation between the start and end values, given the <code>progress</code> parameter.
     */
    template <typename T> inline T InterpolateValue(const T& start, const T& end, float progress) {
        return _interpolate(start, end, progress);
    }
}