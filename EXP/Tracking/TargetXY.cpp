//
//  TargetXY.cpp
//  EXP
//
//  Created by Nick Fagan on 10/28/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#include "TargetXY.hpp"

EXP::TargetXY::TargetXY(EXP::BoundsXY *in_bounds, const EXP::InputXY *in_input_source)
{
    this->bounds = in_bounds;
    this->input_source = in_input_source;
}

EXP::TargetXY::~TargetXY() {};

bool EXP::TargetXY::InBounds() const
{
    return bounds->InBounds(input_source->GetCoordinates());
}

EXP::BoundsXY* EXP::TargetXY::GetBounds() const
{
    return bounds;
}
