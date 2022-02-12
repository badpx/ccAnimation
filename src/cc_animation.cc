#include "cc_animation.h"

namespace anim
{
    void Animation::UpdateState(State new_state, State old_state)
    {
        state_ = new_state;
        if (state_listener_) {
            state_listener_(state_);
        }
    }
}