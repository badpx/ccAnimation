#pragma once
#include <functional>

namespace anim
{

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
            kStarted,
            kPaused,
        };

        virtual ~Animation() = default;

        virtual void SetDuration(long duration) = 0;
        virtual long GetDuration() const = 0;
        void SetDirection(Direction direction) { direction_ = direction; }
        Direction GetDirection() const { return direction_; }

        State GetState() const { return state_; }

        virtual void Start() { state_ = State::kStarted; }
        virtual void Stop() { state_ = State::kStopped; }
        virtual void Cancel() { state_ = State::kStopped; }
        virtual void Pause() { state_ = State::kPaused; }
        virtual void Resume() { state_ = State::kStarted; }

        void SetStateListener(std::function<void(State)> listener) {
            state_listener_ = listener;
        }

        std::function<void(State)> GetStateListener() const {
            return state_listener_;
        }

    protected:
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

    private:
        State state_ = State::kStopped;
        Direction direction_ = Direction::kForward;
        std::function<void(State)> state_listener_;
    };
} // namespace
