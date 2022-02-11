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
            listener_ = listener;
        }

        std::function<void(State)> GetStateListener() const {
            return listener_;
        }

    protected:
        /**
         * @brief This pure virtual function is called every time the animation's currentTime changes.
         *
         * @param currentTime current time in milliseconds.
         */
        virtual void UpdateCurrentTime(long currentTime) = 0;

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
         * @param newState the new state.
         * @param oldState the old state.
         */
        virtual void UpdateState(State newState, State oldState);

    private:
        State state_ = State::kStopped;
        Direction direction_ = Direction::kForward;
        std::function<void(State)> listener_;
    };
} // namespace
