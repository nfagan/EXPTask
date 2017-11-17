//
//  BoundsRectangle.cpp
//  EXP
//
//  Created by Nick Fagan on 10/29/17.
//  Copyright © 2017 Nick Fagan. All rights reserved.
//

#include <EXPTask/Tracking/BoundsRectangle.hpp>

EXP::BoundsRectangle::BoundsRectangle(EXP::Rect<float> bounds) : EXP::BoundsXY()
{
    this->bounds = bounds;
    this->padding = glm::vec2(0.0f);
}

EXP::BoundsRectangle::~BoundsRectangle() {}

void EXP::BoundsRectangle::SetBounds(EXP::Rect<float> bounds)
{
    this->bounds = bounds;
}

bool EXP::BoundsRectangle::InBounds(double x, double y) const
{
    EXP::Rect<float> adjusted_bounds;
    bounds.clone_into(&adjusted_bounds);
    adjusted_bounds.expand(padding.x, padding.y);    
    return x > adjusted_bounds[0] && x < adjusted_bounds[2] &&
        y > adjusted_bounds[1] && y < adjusted_bounds[3];
}

bool EXP::BoundsRectangle::InBounds(glm::vec2 coordinates) const
{
    return InBounds(coordinates.x, coordinates.y);
}
