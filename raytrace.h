#pragma once
#include "common.h"
#include "Image.h"
#include <vector>
#include "object.h"
#include "sphere.h"
#include "floor.h"
#include "triangle.h"
#include "cube.h"
#include "lightsource.h"

/**
 * RayTrace class definition.
 */
class RayTrace
{
public:
    /** RayTrace constructor. */
    RayTrace();

    /**
     * Sets the given pixel to the given colour.
     *
     * @param px - a reference to a pixel
     * @param calcColour - a 3D vector the describes the colour
     */
    void SetColour(Pixel &px, Vector3 calcColour);

    /**
     * Sets the diffuse shade for the given surface.
     *
     * @param surface - the surface to shade
     * @param normal - the surface normal
     * @param aColour - the ambient colour
     * @param dColour - the diffuse colour
     * @param lights - reachable light sources
     * @return a 3D vector containg the calculated colour
     */
    Vector3 DiffuseShade(Vector3 surface, Vector3 normal, Vector3 aColour, Vector3 dColour, std::vector<Vector3> lightsForShading);

    bool CheckShadow(Vector3 intPoint, int objectIndex);

    bool Reflection(Vector3 direction, Vector3 *normal, Vector3 *intPoint, int *minObjectIndex, Vector3 *ambientColour, Vector3 *diffuseColour);

    bool Refraction(Vector3 direction, Vector3 *normal, Vector3 *intPoint, int *refractObjectIndex, Vector3 *refractAmbient, Vector3 *refractDiffuse, bool inside, float *c);

    void FindIntersection();

    /**
     * Ray traces objects and renders an image.
     *
     * @param pImage - the image to render with
     */
    void RayTraceSurface(Image* pImage);

    Vector3 camera;
    Vector3 shadowColour;
    std::vector<Vector3> lightsForShading;
    std::vector<Object*> pObjectList;
    LightSource areaLight;
    Common math;
    int numGridRows;
    int numGridColumns;
    float airIndex;
};

