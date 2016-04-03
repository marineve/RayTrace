#pragma once
#include "object.h"
#include "common.h"

/**
 * Definition for the Floor class.
 * CURRENTLY NOT USED IN THE RAY TRACER
 */
class Floor : public Object
{
public:
    /**
     * Floor class contructor.
     *
     * @param p1 - first vector defining the plane
     * @param p2 - second vector defining the plane
     * @param p3 - third point in the plane
     * @param ambient - ambient colour of the plane
     * @param diffuse - diffuse colour of the plane
     */
    Floor(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 ambient, Vector3 diffuse);

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
    Vector3 normal;
    Common math;
};
