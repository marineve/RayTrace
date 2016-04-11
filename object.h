#pragma once
#include "common.h"
#include "Image.h"

/**
 * Definition for the Object abstract class.
 */
class Object
{
public:
    /**
     * Object class constructor.
     *
     * @param aColour - the ambient colour of the object
     * @param dColour - the diffuse colour of the object
     * @param ref - boolean value indicating whether the object is reflective or not
     */
    Object(Vector3 aColour, Vector3 dColour, bool ref = false, bool refract = false)
    {
        ambientColour = aColour;
        diffuseColour = dColour;
        reflection = ref;
        refraction = refract;
    }

    /**
     * Determines whether a ray from the origin intersects with the object.
     *
     * @param origin - the origin of the intersection ray
     * @param direction - the direction of the ray
     * @param tOut - the t-value
     * @param normalOut - the normal of the intersected surface
     * @param intPoint - the intersection point on the surface
     * @return boolean value indication whether the ray successfully intersected
     */
    virtual bool Intersect(Vector3 origin, Vector3 direction,
                           float* tOut, Vector3* normalOut, Vector3* intPoint, bool backPoint = false) = 0;

    Vector3 ambientColour;
    Vector3 diffuseColour;
    bool reflection;
    bool refraction;
};
