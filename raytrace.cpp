#include "raytrace.h"

RayTrace::RayTrace() {}

void RayTrace::SetColour(Pixel &px, Vector3 calcColour)
{
    if(calcColour.x < 0) px.R = 0;
    else if(calcColour.x > 255) px.R = 255;
    else px.R = (unsigned char)calcColour.x;

    if(calcColour.y < 0) px.G = 0;
    else if(calcColour.y > 255) px.G = 255;
    else px.G = (unsigned char)calcColour.y;

    if(calcColour.z < 0) px.B = 0;
    else if(calcColour.y > 255) px.B = 255;
    else px.B = (unsigned char)calcColour.z;

    px.A = 255;
}

Vector3 RayTrace::DiffuseShade(Vector3 surface, Vector3 normal, Vector3 aColour, Vector3 dColour, std::vector<Vector3> lighting)
{
    //Set the colour to the ambient colour
    Vector3 pixelColour = aColour;

    //Iterate through the reachable light sources
    //Additional light sources are additive; they make everything they reach brighter
    for(int i = 0; i < lighting.size(); i++)
    {
        Vector3 lightVector = math.Minus(lighting[i], surface);
        lightVector = math.Normalize(lightVector);
        float diffuseTerm = math.DotProduct(lightVector, normal);

        if(diffuseTerm > 0)
        {
            Vector3 pixelDiffuseColour = math.MultiplyScalar(dColour, diffuseTerm);
            pixelColour = math.Add(pixelColour, pixelDiffuseColour);
        }
    }

    return pixelColour;
}

void RayTrace::RayTraceSurface(Image* pImage)
{
    //Set up the camera
    Vector3 camera(256, 256, -400);

    //Add desired light sources to the vector
    lights.push_back(Vector3(128, 128, 0));
    lights.push_back(Vector3(384, 128, 128));

    //Define the colour of a shadow
    Vector3 shadowColour = Vector3(0, 0, 0);

    //Initialize a vector of objects
    std::vector<Object*> pObjectList;

    /* Create a cube object
     * Vertices must be added to the vertex list
     * in the order defined by the following diagram.
     *
     *   5___4
     *  /   / |
     * 0___1  |
     * |    | |
     * | 6  | 7
     * |    |/
     * 3___2
     */

    std::vector<Vector3> vertexList;

    vertexList.push_back(Vector3(-256, -256, -1000));
    vertexList.push_back(Vector3(778, -256, -1000));
    vertexList.push_back(Vector3(778, 778, -1000));
    vertexList.push_back(Vector3(-256, 778, -1000));
    vertexList.push_back(Vector3(778, -256, 1000));
    vertexList.push_back(Vector3(-256, -256, 1000));
    vertexList.push_back(Vector3(-256, 778, 1000));
    vertexList.push_back(Vector3(778, 778, 1000));

    Cube cube(CubeConstruct(vertexList), Vector3(0, 100, 0), Vector3(0, 50, 50));
    Sphere sphereOne(Vector3(255, 255, 300), 120, Vector3(40, 20, 20), Vector3(250, 128, 128));

    //Create a reflective surface
    Sphere sphereTwo(Vector3(100, 256, 450), 75, Vector3(40, 20, 20), Vector3(250, 128, 128), true);

    //Add objects to the object list
    pObjectList.push_back(&cube);
    pObjectList.push_back(&sphereOne);
    pObjectList.push_back(&sphereTwo);

    //Apply transformations: translations to the first sphere
    sphereOne.Translate(Vector3(150, 150, 150));

    //Iterate through the pixels
    for(int i = 0; i < 512; i++)
    {
        for(int j = 0; j < 512; j++)
        {
            //Apply super-sampling antialiasing
            //Split each pixel into 4 quarters, then average the colours
            std::vector<Vector3> colours;

            float m = 0;
            while(m < 1)
            {
                float n = 0;
                while(n < 1)
                {
                    //Set up the ray
                    Vector3 pixelPosition((float)j + n, (float)i + m, 0);
                    Vector3 direction = math.Minus(pixelPosition, camera);
                    direction = math.Normalize(direction);

                    float tMin = 999999;
                    Vector3 normalMin;
                    Vector3 intPointMin;
                    int minObjectIndex;
                    bool hasIntersection = false;

                    Vector3 ambientColour;
                    Vector3 diffuseColour;

                    //Determine intersection with the list of objects
                    for(int k = 0; k < pObjectList.size(); k++)
                    {
                        float t;
                        Vector3 normal;
                        Vector3 intPoint;

                        //Shoot a ray in the object direction
                        bool doesIntersect = pObjectList[k]->Intersect(camera, direction, &t, &normal, &intPoint);

                        //If it intersects, determine the colour
                        if(doesIntersect)
                        {
                            hasIntersection = true;
                            if(tMin > t)
                            {
                                tMin = t;
                                normalMin = normal;
                                intPointMin = intPoint;
                                minObjectIndex = k;
                                ambientColour = pObjectList[k]->ambientColour;
                                diffuseColour = pObjectList[k]->diffuseColour;
                            }
                        }
                    }

                    if(hasIntersection)
                    {
                        //Determine if an object is reflective
                        if(pObjectList[minObjectIndex]->reflection)
                        {
                            float newTMin = 999999;
                            Vector3 newIntPointMin;
                            bool didReflection;

                            //Compute a reflection direction
                            float N_dot_V = math.DotProduct(direction, normalMin);
                            Vector3 reflectionDirection = math.MultiplyScalar(math.MultiplyScalar(normalMin, N_dot_V), 2);
                            reflectionDirection = math.Minus(direction, reflectionDirection);

                            //Determine intersection with the list of objects
                            for(int k = 0; k < pObjectList.size(); k++)
                            {
                                float newT;
                                Vector3 newNormal;
                                Vector3 newIntPoint;

                                //Shoot a ray in the reflection direction
                                bool reflection = pObjectList[k]->Intersect(intPointMin, reflectionDirection, &newT, &newNormal, &newIntPoint);

                                //If it hits something, shoot a ray off to the light source to determine colour
                                if(reflection)
                                {
                                    didReflection = true;
                                    if(newTMin > newT)
                                    {
                                        newTMin = newT;
                                        normalMin = newNormal;
                                        newIntPointMin = newIntPoint;
                                        minObjectIndex = k;
                                        ambientColour = pObjectList[k]->ambientColour;
                                        diffuseColour = pObjectList[k]->diffuseColour;
                                    }

                                }
                            }

                            // Check if the reflected ray intersected with any objects.
                            // If it did, then update the intersection point
                            // Otherwise, update that the ray from the cammera intersected with nothing
                            if (didReflection) {
                               intPointMin = newIntPointMin;
                            } else
                            {
                                hasIntersection = false;
                            }
                        }

                        //Determine if a pixel is in shadow from any light
                        bool shadow = true;
                        bool isInLight[lights.size()];

                        for(int k = 0; k < lights.size(); k++)
                        {
                            isInLight[k] = true;
                        }

                        //Iterate through the light sources
                        for(int k = 0; k < lights.size(); k++)
                        {
                            Vector3 lightDirection = math.Minus(lights[k], intPointMin);
                            float lightVectorLength = math.VectorLength(lightDirection);
                            lightDirection = math.Normalize(lightDirection);
                            bool objectShadow = false;

                            //Determine if any objects intersect in front of the light source
                            for(int l = 0; l < pObjectList.size(); l++)
                            {
                                //Only proceed if the object is not intersecting itself
                                if(l != minObjectIndex)
                                {
                                    float t;
                                    Vector3 normal;
                                    Vector3 intPoint;

                                    if(!objectShadow)
                                    {
                                        //Shoot a ray from the object to the light source
                                        bool shadowIntersect = pObjectList[l]->Intersect(intPointMin, lightDirection, &t, &normal, &intPoint);
                                        //Determine pertinent distances
                                        float intPointToLight = math.VectorLength(math.Minus(lights[k], intPoint));
                                        float intersectionPointDistance = math.VectorLength(math.Minus(intPoint, intPointMin));

                                        if(intPointToLight < lightVectorLength && intersectionPointDistance < lightVectorLength)
                                        {
                                            objectShadow = shadowIntersect;

                                            //Determine if a light source should not be used when calculating colour
                                            if(shadowIntersect)
                                            {
                                                isInLight[k] = false;
                                            }
                                        }
                                    }
                                }
                            }
                            shadow &= objectShadow;
                        }

                        //Add usable light sources for determining colour to a vector
                        std::vector<Vector3> lightsForShading;
                        for(int k = 0; k < lights.size(); k++)
                        {
                            if(isInLight[k])
                            {
                                lightsForShading.push_back(lights[k]);
                            }
                        }

                        //When a pixel is not in shadow, determine the colour
                        if(!shadow)
                        {
                            colours.push_back(DiffuseShade(intPointMin, normalMin, ambientColour, diffuseColour, lightsForShading));
                        }
                        else
                        {
                            colours.push_back(shadowColour);
                        }
                    }
                    n += 0.5;
                }
                m += 0.5;
            }

            //Average the four resulting colours for a pixel
            Vector3 result;
            if(colours.size() == 4)
            {
                result = math.VectorAverage(colours[0], colours[1], colours[2], colours[3]);
            }

            //Set the colour of the pixel
            Pixel shade;
            SetColour(shade, result);

            //Render the pixel
            (*pImage)(i, j) = shade;
        }
    }
}
