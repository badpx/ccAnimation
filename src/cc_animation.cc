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
