//
//  Timings.hpp
//  EXP
//
//  Created by Nick Fagan on 10/29/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#ifndef Timings_h
#define Timings_h
#include <limits>
#include <chrono>

namespace EXP {
    namespace Time {
        typedef std::chrono::time_point<std::chrono::steady_clock> time_point;
        typedef std::chrono::duration<double> duration_s;
        typedef std::chrono::duration<double, std::ratio<1, 1000>> duration_ms;
        typedef std::chrono::milliseconds ms;
        typedef std::chrono::seconds secs;
        
        static const duration_s DURATION_INFINITE = duration_s(std::numeric_limits<double>::max());
    }
}

#endif /* Timings_h */
