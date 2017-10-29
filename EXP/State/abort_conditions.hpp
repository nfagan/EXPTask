//
//  abort_conditions.hpp
//  EXP
//
//  Created by Nick Fagan on 10/29/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#ifndef abort_conditions_hpp
#define abort_conditions_hpp

#include <stdio.h>
#include <string>
#include "../Time/Timer.hpp"
#include <EXPGL/Input/InputKeyboard.hpp>

namespace EXP {
    
    //
    //
    //
    
    class State;
    
namespace abort_conditions {
    
    //
    //  base class
    //
    
    class general
    {
    public:
        general() {};
        ~general() {};
        
        virtual bool should_abort(void);
        
        std::string reason;
    };
    
    //
    //  null state
    //
    
    class null_state : public general
    {
    public:
        null_state(EXP::State *state)
        {
            this->state = state;
        }
        bool should_abort(void)
        {
            if (!state)
            {
                reason = "Next state was null.";
                return true;
            }
            return false;
        }
    private:
        EXP::State *state;
    };
    
    //
    //  time exceeded
    //
    
    class time_exceeded : public general
    {
    public:
        time_exceeded(EXP::Time::Timer *timer) : general()
        {
            this->timer = timer;
        }
        bool should_abort(void)
        {
            if (timer->Ellapsed())
            {
                reason = "Time exceeded.";
                return true;
            }
            return false;
        }
    private:
        EXP::Time::Timer *timer;
    };
    
    //
    //  key press
    //
    
    class key_pressed : public general
    {
    public:
        key_pressed(EXP::InputKeyboard *keyboard, int abort_key) : general()
        {
            this->keyboard = keyboard;
            this->abort_key = abort_key;
        }
        
        bool should_abort(void)
        {
            if (keyboard->KeyDown(abort_key))
            {
                reason = "User pressed an abort key: " + std::to_string(abort_key);
                return true;
            }
            return false;
        }
    private:
        EXP::InputKeyboard *keyboard;
        int abort_key;
    };
}
}

#endif /* abort_conditions_hpp */
