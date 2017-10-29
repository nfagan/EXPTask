//
//  Entity.hpp
//  EXP
//
//  Created by Nick Fagan on 10/28/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include <EXPGL/Model/Model.hpp>
#include "../Tracking/TargetXY.hpp"

namespace EXP {
    class Entity
    {
    public:
        Entity();
        ~Entity();
        
        virtual void AttachModel(EXP::Model *model);
        virtual void AttachTarget(EXP::TargetXY *target);
    protected:
        EXP::Model *model;
    };
}

#endif /* Entity_hpp */
