//
//  Task.cpp
//  EXP
//
//  Created by Nick Fagan on 10/29/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#include "Task.hpp"
#include <iostream>
#include <EXPUtil/assert/EXP_ASSERT.h>

EXP::Task::Task(EXP::Time::Keeper *time_keeper) : EXP::StatePrimitive(time_keeper)
{
    n_states = 0;
    AddExitCondition<EXP::exit_conditions::null_next>();
}

EXP::Task::~Task()
{
    for (auto it = states.begin(); it != states.end(); ++it)
    {
        delete it->second;
    }
}

void EXP::Task::OnLoop(std::function<void (Task *)> on_loop)
{
    this->on_loop = on_loop;
}

void EXP::Task::OnExit(std::function<void (Task *)> on_exit)
{
    this->on_exit = on_exit;
}

void EXP::Task::Run()
{
    bool first_entry = true;
    EXP::StatePrimitive *current = next;

    while (!should_exit())
    {

        loop();

        if (first_entry)
        {
            current->entry();
            first_entry = false;
        }

        if (current->should_exit())
        {
            current->exit();
            previous = current;
            next = current->GetNext();
            current = next;
            first_entry = true;
        }
        else
        {
            current->loop();
        }
    }
    
    exit();
}

void EXP::Task::LogTime() const
{
    std::string display_name = get_display_name();
    std::cout << "\nTiming info for task: " << display_name;
    std::cout << "\n\tEllapsed time: " << EllapsedTime().count() << " (s)" << std::endl;
}

void EXP::Task::exit()
{
    on_exit(this);
}

void EXP::Task::loop()
{
    on_loop(this);
}

void EXP::Task::empty()
{
    //
}

EXP::State* EXP::Task::CreateState(unsigned id)
{
    EXP::State *state = new EXP::State(time_keeper.load());
    auto it = states.find(id);
    EXP_ASSERT(it == states.end(), "The given state id `" << id << "` already exists." << std::endl);
    state->set_id(id);
    state->set_parent(this);
    states[id] = state;
    n_states++;
    return state;
}

EXP::State* EXP::Task::GetStateById(unsigned id)
{
    auto it = states.find(id);
    EXP_ASSERT(it != states.end(), "Requested non-existent state `" << id << "`." << std::endl);
    return it->second;
}
