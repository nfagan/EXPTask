//
//  Keeper.hpp
//  EXP
//
//  Created by Nick Fagan on 10/29/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#ifndef Keeper_hpp
#define Keeper_hpp

#include <stdio.h>
#include <atomic>
#include <chrono>
#include <thread>
#include "Timings.hpp"

namespace EXP {
    namespace Time {
        class Keeper
        {
        public:
            Keeper();
            ~Keeper();
            
            void Start(void);
            bool DidStart(void);
            void Stop(void);
            double Now(void) const;
        protected:
            std::thread *time_thread;
            EXP::Time::time_point start;
            std::atomic<double> now;
            std::atomic<bool> is_counting;
            std::atomic<bool> keep_counting;
            void keep_time(void);
        };
    }
}

#endif /* Keeper_hpp */
