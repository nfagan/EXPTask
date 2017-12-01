//
//  exit_conditions.hpp
//  EXPTask
//
//  Created by Nick Fagan on 10/29/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#ifndef exit_conditions_hpp
#define exit_conditions_hpp

#include <stdio.h>
#include <string>
#include <functional>
#include <EXPTask/Time/Timer.hpp>
#include <memory>
#include <EXPGL/Input/InputKeyboard.hpp>

namespace EXP {
    
    //
    //
    //
    
    class StatePrimitive;
    
namespace exit_conditions {
    
    //
    //  base class
    //
    
    class general
    {
    public:
        general() {};
        virtual ~general() {};
        
        virtual bool should_abort(EXP::StatePrimitive *state)
        {
            return true;
        }
        
        std::string reason;
    };
    
    //
    //  custom
    //
    
    class custom : public general
    {
    public:
        custom(std::function<bool (EXP::StatePrimitive*)> abort_func, std::string reason);
        bool should_abort(EXP::StatePrimitive *state);
    private:
        std::function<bool (EXP::StatePrimitive*)> abort_func;
    };
    
    //
    //  abort now
    //
    
    class now : public general
    {
    public:
        now();
        bool should_abort(EXP::StatePrimitive *state);
    };
    
    //
    //  null state
    //
    
    class null_next : public general
    {
    public:
        null_next();
        bool should_abort(EXP::StatePrimitive *state);
    };
    
    //
    //  time exceeded
    //
    
    class time_exceeded : public general
    {
    public:
        time_exceeded(EXP::Time::Timer *timer);
        time_exceeded(void);
        bool should_abort(EXP::StatePrimitive *state);
    private:
        EXP::Time::Timer *timer = nullptr;
    };
    
    //
    //  key press
    //
    
    class key_pressed : public general
    {
    public:
        key_pressed(std::shared_ptr<EXP::InputKeyboard> keyboard, int abort_key);
        bool should_abort(EXP::StatePrimitive *state);
    private:
        std::shared_ptr<EXP::InputKeyboard> keyboard;
        int abort_key;
    };
}
}

#endif /* exit_conditions_hpp */
