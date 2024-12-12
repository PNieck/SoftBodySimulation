#pragma once

#include "materialPoint.hpp"


class Spring {
public:
    Spring(MaterialPoint& anchor1, MaterialPoint& anchor2, const float restLength, const float springCoef):
        restLength(restLength),
        springCoef(springCoef),
        anchorPoint1(anchor1),
        anchorPoint2(anchor2) {}

    float restLength;
    float springCoef;

    MaterialPoint& anchorPoint1;
    MaterialPoint& anchorPoint2;
};