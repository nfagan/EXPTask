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
#include <vector>
#include "../Time/Keeper.hpp"
#include "../Entity/Entity.hpp"
#include "abort_conditions.hpp"

namespace EXP {
    
    class Task;
    
    class State
    {
    public:
        State();
        ~State();
        
        virtual State* Run();
        
        virtual unsigned GetId() const;
        
        template<typename T, typename... A>
        void AddAbortCondition(A... args)
        {
            abort_conditions.push_back(new T(args...));
        };
        
        virtual void SetParent(EXP::Task *parent);
        
    protected:
        unsigned id;
        State *next;
        Task *parent;
        EXP::Entity **entities;
        std::vector<EXP::abort_conditions::general*> abort_conditions;
        
        virtual void entry() {};
        virtual void loop() {};
        virtual void exit() {};
        virtual bool should_exit();
        
        virtual void set_next(State *state);
    };
}

#endif /* State_hpp */
