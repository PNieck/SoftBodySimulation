#pragma once

#include "materialPoint.hpp"


using SpringId = size_t;


class Spring {
public:
    Spring(const MaterialPointId anchor1, const MaterialPointId anchor2, const float restLength, const float springCoef):
        restLength(restLength),
        springCoef(springCoef),
        anchorPoint1(anchor1),
        anchorPoint2(anchor2) {}

    float restLength;
    float springCoef;

    MaterialPointId anchorPoint1;
    MaterialPointId anchorPoint2;
};