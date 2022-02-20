/**
 * @file cc_animation.h
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
#include <algorithm>
#include "cc_animation.h"

namespace anim
{
    long Animation::GetTotalDuration() const {
        auto duration = GetDuration();
        if (duration <= 0) return duration;
        if (loop_count_ < 0) return -1; 
        return duration * loop_count_;
    }

    void Animation::Start() {
        if (state_ == State::kRunning) return;
        SetState(State::kRunning);
    }

    void Animation::Stop() {
        state_ = State::kStopped;
        last_update_time_ = -1;
    }

    void Animation::Cancel() { state_ = State::kStopped; }
    void Animation::Pause() { state_ = State::kPaused; }
    void Animation::Resume() { state_ = State::kRunning; }

    void Animation::SetState(State new_state) {
        if (state_ == new_state) return;
        if (0 == loop_count_) return;
        auto old_state = state_;
        auto old_current_time = current_time_;
        auto old_current_loop = current_loop_;
        auto old_direction = direction_;

        // Check if we should rewind
        if ((State::kPaused == new_state || State::kRunning == new_state)
            && State::kStopped == old_state) {
            // here we reset the time if needed
            //we don't call setCurrentTime because this might change the way the animation
            //behaves: changing the state or changing the current value
            total_current_time_ = current_time_ =
                (direction_ == Direction::kForward) ?  0 : (loop_count_ == -1 ? GetDuration() : GetTotalDuration());
            }

            state_ = new_state;
            // TODO: Register timer.

            UpdateState(new_state, old_state);
            // this is to be safe if updateState changes the state
            if (new_state != state_) return;
            if (State::kRunning == state_ && State::kStopped == old_state) {
                // TODO: Check if is group
                SetCurrentTime(total_current_time_);
            }
    }

    void Animation::UpdateState(State new_state, State old_state)
    {
        state_ = new_state;
        if (state_listener_) {
            state_listener_(state_);
        }
    }

    void Animation::SetCurrentTime(long msecs) {
        msecs = std::max(msecs, 0L);
        // Calculate new time and loop:
        auto duration = GetDuration();
        auto total_duration = duration <= 0 ? duration : (Animation::INFINITE == loop_count_ ? -1 : duration * loop_count_);
        if (total_duration != -1) msecs = std::min(total_duration, msecs);
        total_current_time_ = msecs;
        // Update new values:
        int old_loop = current_loop_;
        current_loop_ = ((duration <= 0) ? 0 : (msecs / duration));
        if (current_loop_ == loop_count_) {
            // At the end loop
            current_time_ = std::max(0L, duration);
            current_loop_ = std::max(0, loop_count_ - 1);
        } else {
            if (direction_ == Direction::kForward) {
                current_time_ = (duration <= 0) ? msecs : (msecs % duration);
            } else {
                current_time_ = (duration <= 0) ? msecs : ((msecs - 1) % duration) + 1;
                if (current_time_ == duration) {
                    --current_loop_;
                }
            }
        }

        UpdateCurrentTime(current_time_);
        if (current_loop_ != old_loop) {
            if (listeners_) {
                // TODO: notify to listeners
            }
        }

        if ((direction_ == Direction::kForward && total_current_time_ == total_duration)
        || (direction_ == Direction::kReverse && total_current_time_ == 0L)) {
            Stop();
        }
    }

    void Animation::UpdateAnimationFrame(long frame_time)
    {
        if (state_ == State::kStopped || -1L == last_update_time_) {
            state_ = State::kRunning;
            last_update_time_ = start_time_ = frame_time;
        }

        if (paused_) {
            if (pause_time_ < 0)
            {
                pause_time_ = frame_time;
            }
        } else if (resumed_) {
            resumed_ = false;
            if (pause_time_ > 0) {
                const long paused_duration = (frame_time - pause_time_);
                start_time_ += paused_duration;
                last_update_time_ = paused_duration;
            }
        }

        // const long current_time = std::max(frame_time, start_time_);
        const long delta = frame_time - last_update_time_;
        last_update_time_ = frame_time;
        if (delta > 0) {
            SetCurrentTime(total_current_time_ + (Direction::kForward == direction_ ? delta : -delta));
        }
    }
}
