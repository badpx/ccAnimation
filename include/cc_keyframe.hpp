/**
 * @file cc_keyframe.h
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

#include "cc_easing_curve.hpp"

namespace anim
{
    template <typename T>
    class Keyframe
    {
    public:
        Keyframe() = default;
        Keyframe(float progress, const T &value, 
                const EasingCurve &curve = EasingCurve(CurveType::Linear))
            : easing_curve_(curve), progress_(progress), value_(value) {}
        float progress() const { return progress_; }
        T value() const { return value_; }
        void set_progress(float progress) { progress_ = progress; }
        void set_value(const T &value) { value_ = value; }
        void set_easing_curve(EasingCurve curve) { easing_curve_ = curve; }
        EasingCurve easing_curve() const { return easing_curve_; }

        // Support operator '<' for sorting
        bool operator<(const Keyframe &other) const { return progress_ < other.progress_; }

    private:
        EasingCurve easing_curve_;
        float progress_;
        T value_;
    };
}   // namespace anim
