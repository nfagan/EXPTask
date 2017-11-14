//
//  TargetSet.hpp
//  EXP
//
//  Created by Nick Fagan on 11/13/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#ifndef TargetSet_hpp
#define TargetSet_hpp

#include "../Tracking/TargetXY.hpp"
#include "../Time/Timings.hpp"
#include "../Time/Timer.hpp"
#include "../Time/Keeper.hpp"
#include <functional>
#include <atomic>

namespace EXP {
    
    class State;
    
    class TargetSet
    {
        friend class State;
    private:
        struct timed_target {
            TargetXY *target;
            Time::Timer timer;
            bool was_in_bounds;
            bool called_ellapsed;
            
            timed_target(TargetXY* in_target, Time::Keeper *keeper, Time::duration_s duration)
            {
                target = in_target;
                timer.Initialize(keeper, duration);
                reset();
            }
            void reset()
            {
                called_ellapsed = false;
                was_in_bounds = false;
                timer.Reset();
            }
        };
        
        TargetSet() = default;
        ~TargetSet() = default;
        
        void initialize(State *parent, Time::Keeper *keeper);
        void update(void);
        void update_one(timed_target &targ);
        void reset(void);
        static void empty(State*, TargetXY*) {};
        
        std::atomic<State*> parent;
        std::atomic<Time::Keeper*> keeper;
        std::vector<timed_target> targets;
        
        std::function<void(State*, TargetXY*)> on_target_exit = &TargetSet::empty;
        std::function<void(State*, TargetXY*)> on_target_entry = &TargetSet::empty;
        std::function<void(State*, TargetXY*)> on_ellapsed = &TargetSet::empty;
        
    public:
        void Add(TargetXY *target, Time::duration_s threshold);
        void OnTargetExit(std::function<void(State*, TargetXY*)> on_target_exit);
        void OnTargetEntry(std::function<void(State*, TargetXY*)> on_target_entry);
        void OnEllapsed(std::function<void(State*, TargetXY*)> on_ellapsed);
    };
}

#endif /* TargetSet_hpp */
