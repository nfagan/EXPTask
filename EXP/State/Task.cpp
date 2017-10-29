//
//  Task.cpp
//  EXP
//
//  Created by Nick Fagan on 10/29/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#include "Task.hpp"

EXP::Task::Task()
{
    this->AddAbortCondition<EXP::abort_conditions::null_state>(this->next);
}

EXP::Task::~Task()
{
    
}

void EXP::Task::AddState(EXP::State *state)
{
    state->SetParent(this);
    states[state->GetId()] = state;
}

EXP::State* EXP::Task::GetStateById(unsigned id)
{
    auto it = states.find(id);
    assert(it != states.end());
    return it->second;
}
