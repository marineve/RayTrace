#pragma once
#include "object.h"
#include "triangle.h"
#include "common.h"
#include <vector>
#include <iostream>

/**
 * Definition of the Cube object class
 */
class Cube : public Object
{
public:
    /**
     * Cube class constructor.
     *
     * @param cube - a struct containing the faces and vertices of the cube
     * @param ambient - the ambient colour of the cube
     * @param diffuse - the diffuse colour of the cube
     * @param reflection - whether the cube is reflective
     */
    Cube(CubeConstruct cube, Vector3 ambient, Vector3 diffuse, bool reflection = false, bool refraction = false);

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
    bool Intersect(Vector3 origin, Vector3 direction,
                   float* tOut, Vector3* normalOut, Vector3* intPointOut, bool backPoint = false);

    /** The triangles that make up the cube. */
    std::vector<Triangle> triangles;
};
