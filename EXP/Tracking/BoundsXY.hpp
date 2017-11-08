//
//  BoundsXY.hpp
//  EXP
//
//  Created by Nick Fagan on 10/28/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#ifndef BoundsXY_hpp
#define BoundsXY_hpp

#include <stdio.h>
#include <glm/glm.hpp>

namespace EXP {
    class BoundsXY
    {
    public:
        BoundsXY(void) {};
        virtual ~BoundsXY(void) {};
        virtual bool InBounds(double x, double y) const { return false; };
        virtual bool InBounds(glm::vec2 coords) const { return false; };
    };
}

#endif /* BoundsXY_hpp */
