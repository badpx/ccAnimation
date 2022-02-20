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
#include <cmath>
#include <initializer_list>
#include <type_traits>
#include <utility>
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
    struct ValueUpdateListener
    {
        virtual void OnUpdate(T value) = 0;
    };

    template <typename T>
    class ValueAnimation : public Animation
    {
        static_assert(std::is_default_constructible<T>::value, "T must have a default constructor!");

    public:

        using ValueSubscriber = std::function<void(const T&)>;
        ValueSubscriber subscriber_;

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
        void set_easing_curve(const EasingCurve &curve) { easing_curve_ = curve; }
        const std::vector<Keyframe<T>> &keyframes() const { return keyframes_; }

    protected:
        void RecalculateCurrentInterval(bool force = false) {
            // can't interpolate if we don't have at least 2 values
            if (keyframes_.size() < 2) return;
            const float end_progress = (direction() == Direction::kForward) ? 1.0f : 0.0f;
            const float progress = easing_curve_.ValueForProgress(((duration_ == 0) ? end_progress : float(GetCurrentTime()) / float(duration_)));
            // 0 and 1 are still the boundaries
            if (force
                || (current_interval_.start.progress() > 0 && progress < current_interval_.start.progress())
                || (current_interval_.end.progress() < 1 && progress > current_interval_.end.progress())) {
                // let's update current_interval_
                auto it = std::lower_bound(keyframes_.cbegin(), keyframes_.cend(),
                                           Keyframe<T>(progress, T()));
                if (it == keyframes_.cbegin()) {
                    // the item pointed to by it is the start element in the range
                    if (it->progress() == 0) {
                        current_interval_.start = *it;
                        current_interval_.end = *(it + 1);
                    }
                } else if (it == keyframes_.cend()) {
                    --it; // position the iterator on the last item
                    if (it->progress() == 1) {
                        // we have an end value (item with progress = 1)
                        current_interval_.start = *(it - 1);
                        current_interval_.end = *it;
                    }
                } else {
                    current_interval_.start = *(it - 1);
                    current_interval_.end = *it;
                }
            }
            SetCurrentValueForProgress(progress);
        }

        void SetCurrentValueForProgress(const float progress) {
            const float start_progress = current_interval_.start.progress();
            const float end_progress = current_interval_.end.progress();
            const float local_progress = (progress - start_progress) / (end_progress - start_progress);
            T ret = InterpolateValue<T>(current_interval_.start.value(), current_interval_.end.value(), local_progress);
            std::swap(current_value_, ret);

            UpdateCurrentValue(current_value_);
            // TODO: notify the value has changed
            if (current_value_ != ret) {
                if (subscriber_) {
                    subscriber_(current_value_);
                }
            }
        }

        virtual void UpdateCurrentValue(const T& value) {
        }

        void UpdateCurrentTime(long current_time) override {
            RecalculateCurrentInterval();
        }

    private:
        std::vector<Keyframe<T>> keyframes_;
        struct {
            Keyframe<T> start, end;
        } current_interval_;
        EasingCurve easing_curve_ = EasingCurve(CurveType::InOutQuad);
        T current_value_;
        long duration_ = 300L;
        bool playing_backwards_ = false;
        int current_iteration_ = 0;
    };
}
