//
//  framer_timer.cpp
//  EXP
//
//  Created by Nick Fagan on 11/13/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#include <stdio.h>

#include <EXPTask/Time/frame_timer.hpp>

EXP::frame_timer::frame_timer()
{
    iterations = 0;
    current = 0;
    last = 0;
    delta = 0;
    mean = 0.0;
    min = std::numeric_limits<double>::max();
    max = std::numeric_limits<double>::min();
}

void EXP::frame_timer::update(const EXP::Time::Keeper *time)
{
    current = time->Now();
    if (iterations > 0)
    {
        delta = current - last;
        double iterations_ = (double) iterations;
        mean = (mean * (iterations_-1.0) + delta) / (iterations_);
        if (delta < min) min = delta;
            if (delta > max) max = delta;
                }
    last = current;
    iterations++;
}
