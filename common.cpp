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

Vector3 Common::VectorAverage(std::vector<Vector3> vList)
{
    Vector3 result;
    for(int i = 0; i < vList.size(); i++) {
        result.x += vList[i].x;
        result.y += vList[i].y;
        result.z += vList[i].z;
    }

    result.x /= vList.size();
    result.y /= vList.size();
    result.z /= vList.size();

    return result;
}

/**
 * @brief Common::GetRotationMatrixForAxis
 *
 * Based on http://inside.mines.edu/fs_home/gmurray/ArbitraryAxisRotation/
 *
 * @param axis
 * @param angle
 * @return
 */
Matrix4x4 Common::GetRotationMatrixForAxis(Vector3 axis, float angle) {
    Matrix4x4 result;
    result.x1 = axis.x * axis.x + (1 - axis.x * axis.x) * cos(angle);
    result.x2 = axis.x * axis.y * (1 - cos(angle)) - axis.z * sin(angle);
    result.x3 = axis.x * axis.z * (1 - cos(angle)) + axis.y * sin(angle);

    result.y1 = axis.x * axis.y * (1 - cos(angle)) + axis.z * sin(angle);
    result.y2 = axis.y * axis.y + (1 - axis.y * axis.y) * cos(angle);
    result.y3 = axis.y * axis.z * (1 - cos(angle)) - axis.x * sin(angle);

    result.z1 = axis.x * axis.z * (1 - cos(angle)) - axis.y * sin(angle);
    result.z2 = axis.y * axis.z * (1 - cos(angle)) + axis.x * sin(angle);
    result.z3 = axis.z * axis.z + (1 - axis.z * axis.z) * cos(angle);

    result.x4 = result.y4 = result.z4 = result.w1 = result.w2 = result.w3 = 0;
    result.w4 = 1;

    return result;
}

