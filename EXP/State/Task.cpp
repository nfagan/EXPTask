//
//  Task.cpp
//  EXP
//
//  Created by Nick Fagan on 10/29/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#include "Task.hpp"
#include <iostream>

EXP::Task::Task(EXP::Time::Keeper *time_keeper) : EXP::State(time_keeper)
{
    n_states = 0;
    this->AddExitCondition<EXP::exit_conditions::null_next>();
}

EXP::Task::~Task()
{
    for (auto it = states.begin(); it != states.end(); ++it)
    {
        delete it->second;
    }
}

void EXP::Task::Run()
{
    while (!should_exit())
    {
        EXP::State *current = next;
        current->run();
        previous = current;
        next = current->GetNext();
    }
}

void EXP::Task::loop()
{
    on_loop(this);
}

EXP::State* EXP::Task::CreateState(unsigned id)
{
    EXP::State *state = new State(time_keeper);
    auto it = states.find(id);
    assert(it == states.end());
    state->set_id(id);
    state->set_parent(this);
    states[id] = state;
    n_states++;
    return state;
}

EXP::State* EXP::Task::GetStateById(unsigned id)
{
    auto it = states.find(id);
    assert(it != states.end());
    return it->second;
}
