//
//  State.hpp
//  EXP
//
//  Created by Nick Fagan on 10/28/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#ifndef State_hpp
#define State_hpp

#include <stdio.h>
#include <functional>
#include <vector>
#include "../Time/Timer.hpp"
#include "../Entity/Entity.hpp"
#include "exit_conditions.hpp"

namespace EXP {
    
    class Task;
    
    class State
    {
        friend class Task;
        
    private:
        State(EXP::Time::Keeper *time_keeper);
        virtual ~State();
        
    public:
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
        virtual unsigned GetId() const;
        virtual State* GetStateById(unsigned id) const;
        State* GetNext() const;
        EXP::Time::Timer* GetTimer() const;
        
        virtual void TimeIn(EXP::Time::duration_s duration);
        virtual void Next(State* state);
        virtual void Entry(std::function<void(State*)> on_entry);
        virtual void Loop(std::function<void(State*)> on_loop);
        virtual void Exit(std::function<void(State*)> on_exit);
        virtual std::string ExitReason(void);
    protected:
        unsigned id;
        EXP::Time::Keeper *time_keeper = nullptr;
        EXP::Time::Timer *timer = nullptr;
        Task *parent = nullptr;
        State *next = nullptr;
        
        std::vector<EXP::exit_conditions::general*> constant_exit_conditions;
        std::vector<EXP::exit_conditions::general*> transient_exit_conditions;
        
        std::function<void(State*)> on_entry = &EXP::State::empty;
        std::function<void(State*)> on_loop = &EXP::State::empty;
        std::function<void(State*)> on_exit = &EXP::State::empty;
        
        EXP::exit_conditions::general *last_exit_condition = nullptr;
        
        virtual void set_id(unsigned id);
        virtual void set_parent(EXP::Task *parent);
        
        virtual void run();
        virtual void empty();
        virtual void entry();
        virtual void loop();
        virtual void exit();
        virtual bool should_exit();
        virtual bool check_exit_conditions(const std::vector<exit_conditions::general*> &exit_conditions);
    };
}

#endif /* State_hpp */
