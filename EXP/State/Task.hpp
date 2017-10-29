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
    public:
        Task();
        ~Task();
        
        void AddState(EXP::State* state);
        EXP::State* GetStateById(unsigned id);
    protected:
        std::unordered_map<unsigned, EXP::State*> states;
    };
}

#endif /* Task_hpp */
