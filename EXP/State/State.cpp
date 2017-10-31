//
//  State.cpp
//  EXP
//
//  Created by Nick Fagan on 10/29/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#include "State.hpp"
#include "Task.hpp"

EXP::State::State(EXP::Time::Keeper *time_keeper)
{
    this->time_keeper = time_keeper;
    this->timer = new EXP::Time::Timer(time_keeper, Time::DURATION_INFINITE);
}

EXP::State::~State()
{
    for (unsigned i = 0; i < constant_exit_conditions.size(); ++i)
    {
        delete constant_exit_conditions[i];
    }
    constant_exit_conditions.clear();
    
    delete timer;
}

void EXP::State::Entry(std::function<void (State *)> on_entry)
{
    this->on_entry = on_entry;
}

void EXP::State::Loop(std::function<void (State *)> on_loop)
{
    this->on_loop = on_loop;
}

void EXP::State::Exit(std::function<void (State *)> on_exit)
{
    this->on_exit = on_exit;
}

void EXP::State::Next(EXP::State *state)
{
    next = state;
}

std::string EXP::State::ExitReason()
{
    if (!last_exit_condition)
    {
        return std::string("");
    }
    return last_exit_condition->reason;
}

void EXP::State::run()
{
    entry();
    while (!should_exit() && !parent->should_exit())
    {
        loop();
    }
    exit();
}

bool EXP::State::should_exit()
{
    if (check_exit_conditions(constant_exit_conditions))
    {
        return true;
    }
    else if (check_exit_conditions(transient_exit_conditions))
    {
        return true;
    }
    
    return false;
}

bool EXP::State::check_exit_conditions(const std::vector<EXP::exit_conditions::general*> &exit_conditions)
{
    for (unsigned i = 0; i < exit_conditions.size(); ++i)
    {
        if (exit_conditions[i]->should_abort(this))
        {
            last_exit_condition = exit_conditions[i];
            return true;
        }
    }
    
    return false;
}

void EXP::State::TimeIn(EXP::Time::duration_s duration)
{
    this->timer->SetDuration(duration);
}

unsigned EXP::State::GetId(void) const
{
    return id;
}

EXP::State* EXP::State::GetNext(void) const
{
    return next;
}

EXP::Time::Timer* EXP::State::GetTimer(void) const
{
    return timer;
}

EXP::State* EXP::State::GetStateById(unsigned id) const
{
    return parent->GetStateById(id);
}

void EXP::State::set_parent(EXP::Task *parent)
{
    this->parent = parent;
}

void EXP::State::set_id(unsigned id)
{
    this->id = id;
}

void EXP::State::entry()
{
    Next(nullptr);
    timer->Reset();
    on_entry(this);
}

void EXP::State::loop()
{
    parent->loop();
    on_loop(this);
}

void EXP::State::exit()
{
    on_exit(this);
    transient_exit_conditions.clear();
}

void EXP::State::empty()
{
    //
}
