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
    class Task : public EXP::State
    {
    
    friend class State;
    
    public:
        Task(EXP::Time::Keeper *keeper);
        virtual ~Task();
        
        virtual void Run(void);
        EXP::State* CreateState(unsigned id);
        EXP::State* GetStateById(unsigned id);
        
    protected:
        EXP::State *previous = nullptr;
        std::unordered_map<unsigned, EXP::State*> states;
        unsigned n_states;
        
        virtual void loop();
    };
}

#endif /* Task_hpp */
