#pragma once
#include "object.h"
#include "common.h"
#include <math.h>

/**
 * Definition for the sphere class.
 */
class Sphere : public Object
{
public:
    /**
     * Constructor for the Sphere class.
     *
     * @param centerIn - the center coordinates of the sphere
     * @param radiusIn - the radius of the sphere
     * @param ambient - the ambient colour of the sphere
     * @param diffuse - the diffuse colour of the sphere
     * @param reflection - whether the surface is reflective
     */
    Sphere(Vector3 centerIn, float radiusIn, Vector3 ambient, Vector3 diffuse, bool reflection = false, bool refraction = false);

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
                           float* tOut, Vector3* normalOut, Vector3* intPoint, bool backPoint = false,
						   Vector3 *aColour = nullptr, Vector3 *dColour = nullptr);

    /**
     * Translates the sphere over the specified distance.
     *
     * @param difference - a 3D vector describing the desired displacement
     */
    void Translate(Vector3 difference);

    /**
     * Transforms the sphere by manipulating a matrix
     * @return a vector containing the new coordinates
     */
    Vector3 Transform();

	std::vector < Vector3 > GetBoundaryPoints() override;

    /** The center of the sphere. */
    Vector3 center;
    /** The radius of the sphere. */
    float radius;
    /** The common math resource to use. */
    Common math;
    /** An array containing the original coordinates. */
    int v[4];
    /** A 2D matrix used for transformations. */
    int m[4][4];
};

