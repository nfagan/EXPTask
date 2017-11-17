//
//  TargetXY.hpp
//  EXP
//
//  Created by Nick Fagan on 10/28/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#ifndef TargetXY_hpp
#define TargetXY_hpp

#include <stdio.h>
#include <EXPGL/Input/InputXY.hpp>
#include <EXPTask/Tracking/BoundsXY.hpp>
#include <atomic>
#include <string>

namespace EXP {
    class TargetXY
    {
    public:
        TargetXY(EXP::BoundsXY *in_bounds, const EXP::InputXY *in_input_source);
        ~TargetXY() = default;
        
        virtual bool InBounds(void) const;
        virtual EXP::BoundsXY* GetBounds(void) const;
        
        unsigned GetId(void) const;
        void SetId(unsigned id);
    protected:
        std::atomic<unsigned int> id;
        EXP::BoundsXY *bounds;
        const EXP::InputXY *input_source = nullptr;
    };
}

#endif /* TargetXY_hpp */
