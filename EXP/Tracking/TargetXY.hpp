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
#include "BoundsXY.hpp"

namespace EXP {
    class TargetXY
    {
    public:
        TargetXY(EXP::BoundsXY *in_bounds, const EXP::InputXY *in_input_source);
        ~TargetXY();
        
        virtual bool InBounds(void) const;
        
        virtual EXP::BoundsXY* GetBounds(void) const;
    protected:
        EXP::BoundsXY *bounds;
        const EXP::InputXY *input_source;
    };
}

#endif /* TargetXY_hpp */
