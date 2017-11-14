//
//  frame_timer.hpp
//  EXP
//
//  Created by Nick Fagan on 11/13/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#ifndef frame_timer_h
#define frame_timer_h

#include "Keeper.hpp"

namespace EXP {
    struct frame_timer
    {
        unsigned long iterations;
        double current;
        double delta;
        double last;
        double min;
        double max;
        double mean;
        
        frame_timer();
        ~frame_timer() = default;
        void update(const EXP::Time::Keeper *time);
    };
}

#endif /* frame_timer_h */
