//
//  StatePrimitive.hpp
//  EXP
//
//  Created by Nick Fagan on 11/4/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#ifndef StatePrimitive_hpp
#define StatePrimitive_hpp

#include <stdio.h>
#include <functional>
#include <vector>
#include <atomic>
#include <EXPGL/Input/InputKeyboard.hpp>
#include "../Time/Timer.hpp"
#include "exit_conditions.hpp"

namespace EXP {
    
    class StatePrimitive
    {
        
    public:
        StatePrimitive(EXP::Time::Keeper *time_keeper);
        virtual ~StatePrimitive();
        
        template<typename T, typename... A>
        void AddExitCondition(A... args)
        {
            constant_exit_conditions.push_back(new T(args...));
        };
        template<typename T, typename... A>
        void AddTransientExitCondition(A... args)
        {
            transient_exit_conditions.push_back(new T(args...));
        };
        
        virtual EXP::Time::Timer* GetTimer() const;
        virtual StatePrimitive* GetNext() const;
        
        virtual EXP::Time::duration_s EllapsedTime() const;
        
        virtual void ExitNow(void);
        virtual void ExitOnTimeExceeded(void);
        virtual void ExitOnKeyPress(InputKeyboard *keyboard, int key);
        virtual void ExitOnceKeyPress(InputKeyboard *keyboard, int key);
        
        virtual void SetTimeIn(EXP::Time::duration_s duration);
        virtual void Next(StatePrimitive* state);
        virtual std::string ExitReason(void);
        
        virtual bool should_exit();
        virtual void entry();
        virtual void loop();
        virtual void exit();
        
    protected:
        std::atomic<EXP::Time::Keeper*> time_keeper;
        std::atomic<EXP::Time::Timer*> timer;
        StatePrimitive *next = nullptr;
        
        std::vector<EXP::exit_conditions::general*> constant_exit_conditions;
        std::vector<EXP::exit_conditions::general*> transient_exit_conditions;
        
        EXP::exit_conditions::general *last_exit_condition = nullptr;
        
        virtual bool check_exit_conditions(const std::vector<exit_conditions::general*> &exit_conditions);
    };
}

#endif /* StatePrimitive_hpp */
