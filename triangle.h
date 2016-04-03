#pragma once
#include "object.h"
#include "common.h"

/**
 * Definition for the Triangle class.
 */
class Triangle : public Object
{
public:
    /**
     * Constructor for the Triangle class.
     * @param p1 - the first point
     * @param p2 - the second point
     * @param p3 - the third point
     * @param ambient - the ambient colour of the triangle
     * @param diffuse - the diffuse colour of the triangle
     * @param reflection - whether the triangle is reflective
     */
    Triangle(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 ambient, Vector3 diffuse, bool reflection = false);

    /**
     * Intersect method inherited from the Object class.
     *
     * @param origin - the origin of the intersection ray
     * @param direction - the direction of the ray
     * @param tOut - the t-value
     * @param normalOut - the normal of the intersected surface
     * @param intPoint - the intersection point on the surface
     * @return boolean value indication whether the ray successfully intersected
     */
    virtual bool Intersect(Vector3 origin, Vector3 direction,
                           float* tOut, Vector3* normalOut, Vector3* intPoint);

    Vector3 point1;
    Vector3 point2;
    Vector3 point3;
    Common math;
    float a, b, c, d, e, f, g, h, i, j, k, l;
};
