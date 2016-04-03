#pragma once
#include "common.h"
#include "Image.h"
#include <vector>
#include "object.h"
#include "sphere.h"
#include "floor.h"
#include "triangle.h"
#include "cube.h"

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
    Vector3 DiffuseShade(Vector3 surface, Vector3 normal, Vector3 aColour, Vector3 dColour, std::vector<Vector3> lights);

    /**
     * Ray traces objects and renders an image.
     *
     * @param pImage - the image to render with
     */
    void RayTraceSurface(Image* pImage);

    /** A vector of available light sources. */
    std::vector<Vector3> lights;
    /** The common vector math library to use. */
    Common math;
};

