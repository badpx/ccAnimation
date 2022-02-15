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
#include <type_traits>
#include <vector>
#include "cc_animation.h"
#include "cc_keyframe.h"

namespace anim
{
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
     * @tparam T The type of start and end value
     * @param start The start value.
     * @param end The end value.
     * @param progress The progress from the starting end the ending values.
     * @return T T A linear interpolation between the start and end values, given the <code>progress</code> parameter.
     */
    template <typename T> inline T InterpolateValue(const T& start, const T& end, float progress) {
        return _interpolate(start, end, progress);
    }

    template <typename T>
    class ValueAnimation : public Animation
    {
        static_assert(std::is_default_constructible<T>::value, "T must have a default constructor!");

    public:
        enum class RepeatMode
        {
            kRestart,
            kReverse,
        };

        ValueAnimation() = delete;
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
        void set_easing_curve(std::shared_ptr<EasingCurve> curve) { easing_curve_ = curve; }

        /**
         * @brief Processes a frame of the animation, adjusting the start time if needed.
         * 
         * @param frame_time  The frame time.
         * @return true  if the animation has ended.
         */
        bool DoAnimationFrame(long frame_time) {

        }

    protected:
        void UpdateCurrentTime(long current_time) override {
        }

        T AnimateValue(float progress)
        {
            size_t num_keyframes = keyframes_.size();
            // can't interpolate if we don't have at least 2 values
            if (num_keyframes < 2) return T();

            Keyframe<T> &first_keyframe = keyframes_[0];
            Keyframe<T> &last_keyframe = keyframes_[num_keyframes - 1];
            // Special-case optimization for the common case of only two keyframes
            if (num_keyframes == 2) {
                auto easing_curve = last_keyframe.easing_curve();
                if (easing_curve != nullptr) {
                    progress = easing_curve->ValueForProgress(progress);
                }
                return InterpolateValue<T>(progress, first_keyframe.value(), last_keyframe.value());
            }

            if (progress <= 0.0f) {
                const Keyframe<T> &next_keyframe = keyframes_[1];
                auto easing_curve = next_keyframe.easing_curve();
                if (easing_curve != nullptr) {
                    progress = easing_curve->ValueForProgress(progress);
                }
                const float prev_progress = first_keyframe.progress();
                float interval_progress = (progress - prev_progress) /
                                         (next_keyframe.progress() - prev_progress);
                return InterpolateValue<T>(interval_progress, first_keyframe.value(), next_keyframe.value());
            }
            else if (progress >= 1.0f)
            {
                const Keyframe<T> &prev_keyframe = keyframes_[num_keyframes - 2];
                auto easing_curve = last_keyframe.easing_curve();
                if (easing_curve != nullptr) {
                    progress = easing_curve->ValueForProgress(progress);
                }
                const float prev_progress = prev_keyframe.progress();
                float interval_progress = (progress - prev_progress) /
                                         (last_keyframe.progress() - prev_progress);
                return InterpolateValue<T>(interval_progress, prev_keyframe.value(), last_keyframe.value());
            }
            /*Keyframe<T> current;
            current.set_progress(progress);
            auto it = std::lower_bound(keyframes_.begin(), keyframes_.end(), current);*/
            Keyframe<T> &prev_keyframe = first_keyframe;
            for (int i = 1; i < num_keyframes; ++i)
            {
                Keyframe<T> &next_keyframe = keyframes_[i];
                if (progress < next_keyframe.progress())
                {
                    const float prev_progress = prev_keyframe.progress();
                    float interval_progress = (progress - prev_progress) /
                                             (next_keyframe.progress() - prev_progress);
                    auto easing_curve = next_keyframe.easing_curve();
                    // Apply interpolator on the proportional duration.
                    if (easing_curve != nullptr) {
                        interval_progress = easing_curve->ValueForProgress(interval_progress);
                    }
                    return InterpolateValue<T>(interval_progress, prev_keyframe.value(), next_keyframe.value());
                }
                prev_keyframe = next_keyframe;
            }
        }

        bool AnimationFrame(long current_time) {

        }

    private:
        std::vector<Keyframe<T>> keyframes_;
        std::shared_ptr<EasingCurve> easing_curve_= std::make_shared<EasingCurve>(CurveType::InOutQuad);
        long duration_;
        RepeatMode repeat_mode_ = RepeatMode::kRestart;
        int repeat_count_ = 0;
    };
}
