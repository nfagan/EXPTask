//
//  StatePrimitive.cpp
//  EXP
//
//  Created by Nick Fagan on 11/4/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#include <EXPTask/State/StatePrimitive.hpp>
#include <iostream>
#include <limits>

EXP::StatePrimitive::StatePrimitive(EXP::Time::Keeper *time_keeper)
{
    this->time_keeper.store(time_keeper);
    this->timer.store(new EXP::Time::Timer(time_keeper, Time::DURATION_INFINITE));
}

EXP::StatePrimitive::~StatePrimitive()
{
    for (unsigned i = 0; i < constant_exit_conditions.size(); ++i)
    {
        delete constant_exit_conditions[i];
    }
    constant_exit_conditions.clear();
    
    delete timer.load();
}

void EXP::StatePrimitive::Next(EXP::StatePrimitive *state)
{
    next = state;
}

void EXP::StatePrimitive::SetTimeIn(EXP::Time::duration_s duration)
{
    GetTimer()->SetDuration(duration);
}

void EXP::StatePrimitive::SetName(std::string name)
{
    this->name = name;
}

EXP::Time::duration_s EXP::StatePrimitive::EllapsedTime() const
{
    return EXP::Time::duration_s(GetTimer()->EllapsedTime());
}

void EXP::StatePrimitive::LogTime() const
{
    std::string display_name = get_display_name();
    std::cout << "\nTiming info for state: " << display_name;
    std::cout << "\n\tNumber of iterations: " << time_descriptives.iterations;
    std::cout << "\n\tLast inter-entry-interval: " << time_descriptives.delta * 1000. << " (ms)";
    std::cout << "\n\tAverage inter-entry-interval: " << time_descriptives.mean * 1000.0 << " (ms)";
    std::cout << "\n\tMax inter-entry-interval: " << time_descriptives.max * 1000.0 << " (ms)";
    std::cout << "\n\tMin inter-entry-interval: " << time_descriptives.min * 1000.0 << " (ms)" << std::endl;
}

EXP::StatePrimitive* EXP::StatePrimitive::GetNext() const
{
    return next;
}

const std::string& EXP::StatePrimitive::GetName() const
{
    return name;
}

std::string EXP::StatePrimitive::ExitReason()
{
    if (!last_exit_condition)
    {
        return std::string("");
    }
    return last_exit_condition->reason;
}

void EXP::StatePrimitive::ExitNow()
{
    AddTransientExitCondition<exit_conditions::now>();
}

void EXP::StatePrimitive::ExitOnTimeExceeded()
{
    AddExitCondition<exit_conditions::time_exceeded>(GetTimer());
}

void EXP::StatePrimitive::ExitOnKeyPress(const std::shared_ptr<EXP::InputKeyboard> &keyboard, int key)
{
    AddExitCondition<exit_conditions::key_pressed>(keyboard, key);
}

EXP::Time::Timer* EXP::StatePrimitive::GetTimer(void) const
{
    return timer.load();
}

bool EXP::StatePrimitive::should_exit()
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

bool EXP::StatePrimitive::check_exit_conditions(const std::vector<EXP::exit_conditions::general*> &exit_conditions)
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

std::string EXP::StatePrimitive::get_display_name() const
{
    if (name.empty())
    {
        return "(unnamed)";
    }
    return name;
}

void EXP::StatePrimitive::entry()
{
    Next(nullptr);
    GetTimer()->Reset();
    time_descriptives.update(time_keeper);
}

void EXP::StatePrimitive::loop()
{
    //
}

void EXP::StatePrimitive::exit()
{
    transient_exit_conditions.clear();
}

