//
//  State.cpp
//  EXP
//
//  Created by Nick Fagan on 10/29/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#include <EXPTask/State/State.hpp>
#include <EXPTask/State/Task.hpp>

EXP::State::State(EXP::Time::Keeper *time_keeper) : EXP::StatePrimitive(time_keeper)
{
    target_set.initialize(this, time_keeper);
}

void EXP::State::OnEntry(std::function<void (State *)> on_entry)
{
    this->on_entry = on_entry;
}

void EXP::State::OnLoop(std::function<void (State *)> on_loop)
{
    this->on_loop = on_loop;
}

void EXP::State::OnExit(std::function<void (State *)> on_exit)
{
    this->on_exit = on_exit;
}

void EXP::State::Next(EXP::State *state)
{
    next = state;
}

void EXP::State::run()
{
    entry();
    while (!should_exit() && !parent->should_exit())
    {
        this->loop();
    }
    exit();
}

EXP::TargetSet& EXP::State::GetTargetSet()
{
    return target_set;
}

unsigned EXP::State::GetId(void) const
{
    return id;
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
    EXP::StatePrimitive::entry();
    target_set.reset();
    on_entry(this);
}

void EXP::State::loop()
{
    target_set.update();
    on_loop(this);
}

void EXP::State::exit()
{
    on_exit(this);
    EXP::StatePrimitive::exit();
}

void EXP::State::empty()
{
    //
}
