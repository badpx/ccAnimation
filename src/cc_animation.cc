#include "cc_animation.h"

namespace anim
{
    void Animation::UpdateState(State newState, State oldState)
    {
        state_ = newState;
        if (listener_) {
            listener_(state_);
        }
    }
}