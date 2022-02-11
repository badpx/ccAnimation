#pragma once

#include "cc_animation.h"

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

        void SetDuration(long duration) override { duration_ = duration; }
        long GetDuration() const override { return duration_; }
        int GetRepeatCount() const { return repeat_count_; }
        void SetRepeatCount(int count) { repeat_count_ = count; }
        RepeatMode GetRepeatMode() const { return repeat_mode_; }
        void SetRepeatMode(RepeatMode mode) { repeat_mode_ = mode; }

    protected:
        void UpdateCurrentTime(long currentTime) override;

    private:
        long duration_;
        int repeat_count_ = 0;
        RepeatMode repeat_mode_ = RepeatMode::kRestart;
    };
}