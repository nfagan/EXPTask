//
//  Keeperr.cpp
//  EXP
//
//  Created by Nick Fagan on 10/29/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#include "Timer.hpp"
#include <iostream>
#include <math.h>
#include <assert.h>

EXP::Time::Timer::Timer(EXP::Time::Keeper *time, EXP::Time::duration_ms duration)
{
    this->time = time;
    this->duration = duration;
    start = time->Now();
    assert(!isnan(start));
}

EXP::Time::Timer::~Timer() {};

bool EXP::Time::Timer::Ellapsed()
{
    double ellapsed_seconds = time->Now() - start;
    EXP::Time::duration_s duration_s = EXP::Time::duration_s(duration);
    return ellapsed_seconds >= duration_s.count();
}

void EXP::Time::Timer::Reset()
{
    start = time->Now();
}
