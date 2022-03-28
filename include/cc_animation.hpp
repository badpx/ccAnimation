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
#pragma once
#include <functional>
#include <memory>
#include <list>

namespace anim
{
    class AnimationListener;
    class Animation
    {
    public:
        enum class Direction
        {
            kForward,
            kReverse,
        };

        enum class State
        {
            kStopped,
            kRunning,
            kPaused,
        };

        enum class LoopMode
        {
            kRestart,
            kReverse,
        };

        static const int INFINITE = -1;

        virtual ~Animation() = default;

        virtual void SetDuration(long duration) = 0;
        virtual long GetDuration() const = 0;
        long GetTotalDuration() const;
        void set_direction(Direction direction) { direction_ = direction; }
        Direction direction() const { return direction_; }
        int loop_count() const { return loop_count_; }
        void set_loop_count(int count) { loop_count_ = count; }
        LoopMode loop_mode() const { return loop_mode_; }
        void set_loop_mode(LoopMode mode) { loop_mode_ = mode; }

        State state() const { return state_; }

        virtual void Start();
        virtual void Stop();
        virtual void Cancel();
        virtual void Pause();
        virtual void Resume();

        void SetStateListener(std::function<void(State)> listener) {
            state_listener_ = listener;
        }

        std::function<void(State)> GetStateListener() const {
            return state_listener_;
        }

        void AddAnimationListener(const std::shared_ptr<AnimationListener>& listener) {
            if (nullptr == listeners_) {
                listeners_ = decltype(listeners_)(new std::list<std::shared_ptr<AnimationListener>>());
            }
            listeners_->push_back(listener);
        }

        void RemoveAnimationListener(const std::shared_ptr<AnimationListener>& listener) {
            if (listeners_) {
                listeners_->remove_if([&listener](const std::shared_ptr<AnimationListener>& n) -> bool { return n == listener; });
            }
        }

        void SetCurrentTime(long msecs);
        long GetCurrentTime() const { return current_time_; }

        /**
         * @brief Processes a frame of the animation, adjusting the start time if needed.
         * 
         * @param frame_time  The frame time.
         * @return true  if the animation has ended.
         */
        void UpdateAnimationFrame(long frame_time);

    protected:
        void SetState(State state);
        /**
         * @brief This pure virtual function is called every time the animation's currentTime changes.
         *
         * @param current_time current time in milliseconds.
         */
        virtual void UpdateCurrentTime(long current_time) = 0;

        /**
         * @brief This virtual function is called by Animation when the direction of the animation
         * is changed. The direction argument is the new direction.
         *
         * @param direction the new direction.
         */
        virtual void UpdateDirection(Direction direction) { direction_ = direction; }
        /**
         * @brief This virtual function is called by Animation when the state of the animation
         * is changed from oldState to newState.
         *
         * @param new_state the new state.
         * @param old_state the old state.
         */
        virtual void UpdateState(State new_state, State old_state);

        std::unique_ptr<std::list<std::shared_ptr<AnimationListener>>> listeners_ = nullptr;
        bool paused_ = false;
        State state_ = State::kStopped;
    // private:
        Direction direction_ = Direction::kForward;
        std::function<void(State)> state_listener_;
        LoopMode loop_mode_ = LoopMode::kRestart;
        long start_time_ = 0L;
        long pause_time_ = 0L;
        long last_update_time_ = -1L;
        long current_time_ = 0L;
        long total_current_time_ = 0L;
        int loop_count_ = 1;
        int current_loop_ = 0;
        bool resumed_ = false;
    };

    struct AnimationListener {
        virtual void OnAnimationStart(Animation &animation) {}
        virtual void OnAnimationEnd(Animation &animation) {}
        virtual void OnAnimationCancel(Animation &animation) {}
        virtual void OnAnimationRepeat(Animation &animation) {}
        virtual void OnAnimationPause(Animation &animation) {}
        virtual void OnAnimationResume(Animation &animation) {}
    };
} // namespace
