#pragma once

namespace anim {

    enum class AnimationDirection {
        kForward,
        kReverse,
    };

    enum class AnimationState {
        kStopped,
        kStarted,
        kPaused,
    };
     
    class Animation {
        public:

            virtual ~Animation() = default;

            virtual void SetDuration(long duration) = 0;
            virtual long GetDuration() = 0;

            AnimationState GetState() { return state_; }

            virtual void Start() { state_ = AnimationState::kStarted; }
            virtual void Stop()  { state_ = AnimationState::kStopped; }
            virtual void Pause() { state_ = AnimationState::kPaused; }
            virtual void Resume(){ state_ = AnimationState::kStarted; }

        private:
            AnimationState state_ = AnimationState::kStopped;
    };
}   // namespace
