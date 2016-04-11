#include "lightsource.h"

LightSource::LightSource(Vector3 x, Vector3 y, Vector3 z, float n_in)
{
    corner.x = x.x;
    corner.y = x.y;
    corner.z = x.z;
    vec_a = math.Minus(x, y);
    vec_b = math.Minus(x, z);
    n = n_in;
}
