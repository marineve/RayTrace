#pragma once
#include "Common.h"

class LightSource
{
public:
    LightSource(Vector3 x, Vector3 y, Vector3 z, float n_in);

    Common math;
    Vector3 corner;
    Vector3 vec_a;
    Vector3 vec_b;
    int n;
};
