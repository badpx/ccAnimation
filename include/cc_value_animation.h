#pragma once

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

        void SetDuration(long duration) override { duration_ = duration; }
        long GetDuration() const override { return duration_; }
        int GetRepeatCount() const { return repeat_count_; }
        void SetRepeatCount(int count) { repeat_count_ = count; }
        RepeatMode GetRepeatMode() const { return repeat_mode_; }
        void SetRepeatMode(RepeatMode mode) { repeat_mode_ = mode; }

    protected:
        void UpdateCurrentTime(long current_time) override {
        }

        void AnimateValue(float fraction) {
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
        long duration_;
        int repeat_count_ = 0;
        RepeatMode repeat_mode_ = RepeatMode::kRestart;
    };
}