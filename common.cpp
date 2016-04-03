#include "common.h"
#include <math.h>

Common::Common() {}

Vector3 Common::Add(Vector3 a, Vector3 b)
{
    return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3 Common::Minus(Vector3 a, Vector3 b)
{
    return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector3 Common::MultiplyScalar(Vector3 a, float b)
{
    return Vector3(a.x * b, a.y * b, a.z * b);
}

float Common::DotProduct(Vector3 a, Vector3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Common::Normalize(Vector3 a)
{
    float length = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
    return Vector3(a.x/length, a.y/length, a.z/length);
}

Vector3 Common::CrossProduct(Vector3 a, Vector3 b)
{
    return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

float Common::VectorLength(Vector3 a)
{
    return sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));
}

Vector3 Common::VectorAverage(Vector3 a, Vector3 b, Vector3 c, Vector3 d)
{
    Vector3 result;
    result.x = (a.x + b.x + c.x + d.x)/4;
    result.y = (a.y + b.y + c.y + d.y)/4;
    result.z = (a.z + b.z + c.z + d.z)/4;
    return result;
}

