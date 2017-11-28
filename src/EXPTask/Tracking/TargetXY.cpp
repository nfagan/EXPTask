//
//  TargetXY.cpp
//  EXP
//
//  Created by Nick Fagan on 10/28/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#include <EXPTask/Tracking/TargetXY.hpp>

EXP::TargetXY::TargetXY(std::shared_ptr<EXP::BoundsXY> in_bounds, const std::shared_ptr<EXP::InputXY> in_input_source)
{
    id.store(0);
    this->bounds = in_bounds;
    this->input_source = in_input_source;
}

bool EXP::TargetXY::InBounds() const
{
    return bounds->InBounds(input_source->GetCoordinates());
}

std::shared_ptr<EXP::BoundsXY>& EXP::TargetXY::GetBounds()
{
    return bounds;
}

void EXP::TargetXY::SetId(unsigned id)
{
    this->id.store(id);
}

unsigned EXP::TargetXY::GetId() const
{
    return id.load();
}
