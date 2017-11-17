//
//  TargetSet.cpp
//  EXP
//
//  Created by Nick Fagan on 11/13/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#include <EXPTask/State/TargetSet.hpp>
#include <iostream>

void EXP::TargetSet::initialize(EXP::State *parent, Time::Keeper *keeper)
{
    this->parent.store(parent);
    this->keeper.store(keeper);
}

void EXP::TargetSet::Add(EXP::TargetXY *target, Time::duration_s threshold)
{
    targets.push_back(timed_target(target, keeper.load(), threshold));
}

void EXP::TargetSet::OnTargetEntry(std::function<void(State*, TargetXY*)> on_target_entry)
{
    this->on_target_entry = on_target_entry;
}

void EXP::TargetSet::OnTargetExit(std::function<void(State*, TargetXY*)> on_target_exit)
{
    this->on_target_exit = on_target_exit;
}

void EXP::TargetSet::OnEllapsed(std::function<void(State*, TargetXY*)> on_ellapsed)
{
    this->on_ellapsed = on_ellapsed;
}

void EXP::TargetSet::reset()
{
    for (unsigned i = 0; i < targets.size(); ++i)
    {
        targets[i].reset();
    }
}

void EXP::TargetSet::update()
{
    for (unsigned i = 0; i < targets.size(); ++i)
    {
        update_one(targets[i]);
    }
}

void EXP::TargetSet::update_one(EXP::TargetSet::timed_target &target)
{
    if (target.target->InBounds())
    {
        if (!target.was_in_bounds)
        {
            on_target_entry(parent, target.target);
            target.was_in_bounds = true;
        }
    }
    else
    {
        target.timer.Reset();
        if (target.was_in_bounds)
        {
            on_target_exit(parent, target.target);
            target.was_in_bounds = false;
        }
    }
    
    if (target.timer.Ellapsed() && !target.called_ellapsed)
    {
        on_ellapsed(parent, target.target);
        target.called_ellapsed = true;
    }
}
