//
//  Task.hpp
//  EXP
//
//  Created by Nick Fagan on 10/29/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#ifndef Task_hpp
#define Task_hpp

#include <stdio.h>
#include "State.hpp"
#include <unordered_map>

namespace EXP {
    class Task : public StatePrimitive
    {
    
    public:
        Task(EXP::Time::Keeper *keeper);
        virtual ~Task();
        
        virtual void Run(void);
        virtual void OnLoop(std::function<void(Task*)> on_loop);
        virtual void OnExit(std::function<void(Task*)> on_exit);
        
        EXP::State* CreateState(unsigned id);
        EXP::State* GetStateById(unsigned id);
        
    protected:
        EXP::StatePrimitive *previous = nullptr;
        std::unordered_map<unsigned, EXP::State*> states;
        unsigned n_states;
        
        std::function<void(Task*)> on_loop = &EXP::Task::empty;
        std::function<void(Task*)> on_exit = &EXP::Task::empty;
        
        virtual void empty();
        virtual void loop();
        virtual void exit();
    };
}

#endif /* Task_hpp */
