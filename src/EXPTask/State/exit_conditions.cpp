//
//  exit_conditions.cpp
//  EXP
//
//  Created by Nick Fagan on 10/30/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#include <stdio.h>
#include <EXPTask/State/exit_conditions.hpp>
#include <EXPTask/State/StatePrimitive.hpp>
#include <iostream>

namespace EXP {
    namespace exit_conditions {
        
        //
        //  now
        //
        
        now::now() : general::general() {};
        bool now::should_abort(EXP::StatePrimitive *state)
        {
            reason = "State exit condition met: user manually invoked abort.";
            return true;
        }
        
        //
        //  custom
        //
        
        custom::custom(std::function<bool (EXP::StatePrimitive*)> abort_func, std::string reason) : general::general()
        {
            this->abort_func = abort_func;
            this->reason = reason;
        }
        bool custom::should_abort(EXP::StatePrimitive *state)
        {
            return abort_func(state);
        }
        
        //
        //  next state is null
        //
        
        null_next::null_next() : general::general() {};
        bool null_next::should_abort(EXP::StatePrimitive *state)
        {
            if (!state->GetNext())
            {
                reason = "State exit condition met: Next state was null.";
                return true;
            }
            return false;
        }
        
        //
        //  time exceeded
        //
        
        time_exceeded::time_exceeded(EXP::Time::Timer *timer) : general::general()
        {
            this->timer = timer;
        }
        time_exceeded::time_exceeded(void) : general::general() {};
        bool time_exceeded::should_abort(EXP::StatePrimitive *state)
        {
            if (!timer)
            {
                timer = state->GetTimer();
            }
            if (timer->Ellapsed())
            {
                reason = "State exit condition met: time in state exceeded.";
                return true;
            }
            return false;
        }
        
        //
        //  key pressed
        //
        
        key_pressed::key_pressed(EXP::InputKeyboard *keyboard, int abort_key) : general::general()
        {
            this->keyboard = keyboard;
            this->abort_key = abort_key;
        }
        
        bool key_pressed::should_abort(EXP::StatePrimitive *state)
        {
            if (keyboard->KeyDown(abort_key))
            {
                reason = "State exit condition met: user pressed an abort key: " + std::to_string(abort_key);
                return true;
            }
            return false;
        }
    }
}
