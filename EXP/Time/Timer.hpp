//
//  Keeperr.hpp
//  EXP
//
//  Created by Nick Fagan on 10/29/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#ifndef Keeperr_hpp
#define Keeperr_hpp

#include <stdio.h>
#include "Keeper.hpp"

namespace EXP {
    namespace Time {
        class Timer
        {
        public:
            Timer(EXP::Time::Keeper *time, EXP::Time::duration_ms duration);
            ~Timer();
            
            void Reset(void);
            bool Ellapsed(void);
        protected:
            double start;
            EXP::Time::Keeper *time;
            EXP::Time::duration_ms duration;
        };
    }
}

#endif /* Keeperr_hpp */
