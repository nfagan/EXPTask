//
//  Keeperr.cpp
//  EXP
//
//  Created by Nick Fagan on 10/29/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#include <EXPTask/Time/Timer.hpp>
#include <iostream>
#include <math.h>
#include <assert.h>

EXP::Time::Timer::Timer(EXP::Time::Keeper *time)
{
    this->time = time;
    this->duration = DURATION_INFINITE;
    init_time();
}

EXP::Time::Timer::Timer(EXP::Time::Keeper *time, EXP::Time::duration_ms duration)
{
    this->time = time;
    this->duration = duration;
    init_time();
}

void EXP::Time::Timer::Initialize(EXP::Time::Keeper *time, EXP::Time::duration_ms duration)
{
    this->time = time;
    this->duration = duration;
    init_time();
}

void EXP::Time::Timer::init_time()
{
    assert(!did_initialize);
    start = time->Now();
    assert(!isnan(start));
    did_initialize = true;
}

void EXP::Time::Timer::SetDuration(EXP::Time::duration_ms duration)
{
    this->duration = duration;
}

bool EXP::Time::Timer::Ellapsed() const
{
    double ellapsed_seconds = EllapsedTime();
    EXP::Time::duration_s duration_s = EXP::Time::duration_s(duration);
    return ellapsed_seconds >= duration_s.count();
}

double EXP::Time::Timer::EllapsedTime() const
{
    return time->Now() - start;
}

void EXP::Time::Timer::Reset()
{
    start = time->Now();
}
