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
        virtual bool InBounds(double x, double y) { return false; };
        virtual bool InBounds(glm::vec2 coords) { return false; };
//        virtual void MoveTo(double x, double y);
//        virtual void SetPadding(double x, double y);
    };
}

#endif /* BoundsXY_hpp */
