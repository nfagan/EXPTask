//
//  BoundsRectangle.hpp
//  EXP
//
//  Created by Nick Fagan on 10/29/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#ifndef BoundsRectangle_hpp
#define BoundsRectangle_hpp

#include <stdio.h>
#include <EXPGL/Util/Rect.hpp>
#include "BoundsXY.hpp"

namespace EXP {
    class BoundsRectangle : public BoundsXY
    {
    public:
        BoundsRectangle(EXP::Rect<float> bounds);
        ~BoundsRectangle();
        
        bool InBounds(double x, double y);
        bool InBounds(glm::vec2 coordinates);
        
        template<typename T>
        void SetBounds(EXP::Rect<T> bounds)
        {
            this->bounds = static_cast<EXP::Rect<float>>(bounds);
        }
    protected:
        EXP::Rect<float> bounds;
        glm::vec2 padding;
    };
}

#endif /* BoundsRectangle_hpp */
