//
//  State.cpp
//  EXP
//
//  Created by Nick Fagan on 10/29/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#include "State.hpp"

EXP::State::State()
{
    
}

EXP::State::~State()
{
    abort_conditions.clear();
}

EXP::State* EXP::State::Run()
{
    entry();
    while (!should_exit())
    {
        loop();
    }
    exit();
    return next;
}

bool EXP::State::should_exit()
{
    for (unsigned i = 0; i < abort_conditions.size(); ++i)
    {
        if (abort_conditions[i]->should_abort())
        {
            return true;
        }
    }
    return false;
}

unsigned EXP::State::GetId(void) const
{
    return id;
}

void EXP::State::SetParent(EXP::Task *parent)
{
    this->parent = parent;
}

void EXP::State::set_next(State *state)
{
    next = state;
}
