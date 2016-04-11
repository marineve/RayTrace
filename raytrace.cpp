#include "raytrace.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

RayTrace::RayTrace():
    camera(256, 256, -400),
    shadowColour(0, 0, 0),
    areaLight(Vector3(300, -200, -50), Vector3(100, -200, -50), Vector3(300, -200, 50), 2) {}

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

Vector3 RayTrace::DiffuseShade(Vector3 surface, Vector3 normal, Vector3 aColour, Vector3 dColour, std::vector<Vector3> lightsForShading)
{
    float k_a = 0.7;
    float k_d = 0.8;

    //Set the colour to the ambient colour
    Vector3 pixelColour = math.MultiplyScalar(aColour, k_a);
    Vector3 totalColour;

    //Iterate through the reachable light sources
    for(int i = 0; i < lightsForShading.size(); i++) {
        Vector3 lightVector = math.Minus(lightsForShading[i], surface);
        lightVector = math.Normalize(lightVector);
        float diffuseTerm = math.DotProduct(lightVector, normal);

        if(diffuseTerm > 0)
        {
            Vector3 pixelDiffuseColour = math.MultiplyScalar(dColour, diffuseTerm);
            pixelDiffuseColour = math.MultiplyScalar(pixelDiffuseColour, k_d);
            Vector3 colour = math.Add(pixelColour, pixelDiffuseColour);
            totalColour = math.Add(totalColour, colour);
        } else {
            totalColour = math.Add(totalColour, pixelColour);
        }
    }

    return math.MultiplyScalar(totalColour, 1.0/(areaLight.n*areaLight.n));
}

bool RayTrace::CheckShadow(Vector3 intPoint, int objectIndex)
{
    bool shadow = true;

    for(int i = 0; i < areaLight.n; i++)
    {
        for(int j = 0; j < areaLight.n; j++)
        {
            //Find a point light in the area light
            float xi1 = rand() / (float) RAND_MAX;
            float xi2 = rand() / (float) RAND_MAX;
            Vector3 r = math.Add(areaLight.corner, math.MultiplyScalar(areaLight.vec_a, xi1));
            r = math.Add(r, math.MultiplyScalar(areaLight.vec_b, xi2));

            Vector3 lightDirection = math.Minus(r, intPoint);
            float lightVectorLength = math.VectorLength(lightDirection);
            lightDirection = math.Normalize(lightDirection);
            bool objectShadow = false;

            //Determine if any objects intersect in front of the light source
            for(int k = 0; k < pObjectList.size(); k++)
            {
                //Only proceed if the object is not intersecting with itself and the object iterated over is not refractive
                if(k != objectIndex && !pObjectList[k]->refraction)
                {
                    float t;
                    Vector3 normal;
                    Vector3 intPointShadow;

                    //Shoot a ray from the object to the light source
                    bool shadowIntersect = pObjectList[k]->Intersect(intPoint, lightDirection, &t, &normal, &intPointShadow);

                    //Determine pertinent distances
                    float intPointToLight = math.VectorLength(math.Minus(r, intPointShadow));
                    float intersectionPointDistance = math.VectorLength(math.Minus(intPointShadow, intPoint));

                    if(intPointToLight < lightVectorLength && intersectionPointDistance < lightVectorLength)
                    {
                        objectShadow = shadowIntersect;
                    }

                    if (objectShadow) {
                        break;
                    }
                }
            }
            shadow &= objectShadow;
            if(!objectShadow) {
                lightsForShading.push_back(r);
            }
        }
    }
    return shadow;
}

bool RayTrace::Reflection(Vector3 direction, Vector3 *normal, Vector3 *intPoint,
                          int *minObjectIndex, Vector3 *ambientColour, Vector3 *diffuseColour)
{
    float tMin = 999999;
    Vector3 intPointMin;
    Vector3 normalMin;
    bool didReflect;

    //Compute a reflection direction
    float N_dot_V = math.DotProduct(direction, *normal);
    Vector3 reflectionDirection = math.MultiplyScalar(math.MultiplyScalar(*normal, N_dot_V), 2);
    reflectionDirection = math.Minus(direction, reflectionDirection);

    //Determine intersection with the list of objects
    for(int i = 0; i < pObjectList.size(); i++)
    {
        float t;
        Vector3 newNormal;
        Vector3 newIntPoint;

        //Shoot a ray in the reflection direction
        bool reflection = pObjectList[i]->Intersect(*intPoint, reflectionDirection, &t, &newNormal, &newIntPoint);

        //If it hits something, shoot a ray off to the light source to determine colour
        if(reflection)
        {
            didReflect = true;
            if(tMin > t)
            {
                tMin = t;
                normalMin = newNormal;
                if (i == 1) {
                    normalMin = math.MultiplyScalar(normalMin, -1);
                }
                intPointMin = newIntPoint;
                *minObjectIndex = i;
                *ambientColour = pObjectList[i]->ambientColour;
                *diffuseColour = pObjectList[i]->diffuseColour;
            }

        }
    }

    // Check if the reflected ray intersected with any objects.
    // If it did, then update the intersection point
    // Otherwise, update that the ray from the cammera intersected with nothing
    if (didReflect) {
        *intPoint = intPointMin;
        *normal = normalMin;
        return true;
    }
    else
    {
        return false;
    }
}

bool RayTrace::Refraction(Vector3 dir, Vector3 norm, float rindex, Vector3* t)
{
    //Find the direction of the ray in the refractive material
    float d_dot_n = math.DotProduct(dir, norm);
    Vector3 exp1 = math.MultiplyScalar(norm, d_dot_n);
    exp1 = math.Minus(dir, exp1);
    exp1 = math.MultiplyScalar(exp1, airIndex/rindex);

    float temp = d_dot_n * d_dot_n;
    temp = 1 - temp;
    temp = ((airIndex * airIndex)/(rindex * rindex)) * temp;
    temp = 1 - temp;

    if(temp < 0) {
        return false;
    }

    temp = sqrt(temp);
    Vector3 exp2 = math.MultiplyScalar(norm, temp);
    *t = math.Minus(exp1, exp2);


    return true;
}

void FindIntersection() {

}

void RayTrace::RayTraceSurface(Image* pImage)
{

    /* Create a cube room to surround the scene
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

    std::vector<Vector3> cubeRoomVertices;

    cubeRoomVertices.push_back(Vector3(-256, -256, -1000));
    cubeRoomVertices.push_back(Vector3(778, -256, -1000));
    cubeRoomVertices.push_back(Vector3(778, 778, -1000));
    cubeRoomVertices.push_back(Vector3(-256, 778, -1000));
    cubeRoomVertices.push_back(Vector3(778, -256, 1000));
    cubeRoomVertices.push_back(Vector3(-256, -256, 1000));
    cubeRoomVertices.push_back(Vector3(-256, 778, 1000));
    cubeRoomVertices.push_back(Vector3(778, 778, 1000));

    Cube cubeRoom(CubeConstruct(cubeRoomVertices), Vector3(0, 100, 0), Vector3(0, 50, 50));

    std::vector<Vector3> cubeVertices;

    cubeVertices.push_back(Vector3(425, 425, 50));
    cubeVertices.push_back(Vector3(475, 425, 50));
    cubeVertices.push_back(Vector3(475, 475, 50));
    cubeVertices.push_back(Vector3(425, 475, 50));
    cubeVertices.push_back(Vector3(475, 425, 100));
    cubeVertices.push_back(Vector3(425, 425, 100));
    cubeVertices.push_back(Vector3(425, 475, 100));
    cubeVertices.push_back(Vector3(475, 475, 100));

    Cube cube(CubeConstruct(cubeVertices), Vector3(0, 0, 150), Vector3(0, 0, 200));

    Sphere sphereOne(Vector3(255, 255, 300), 120, Vector3(40, 20, 20), Vector3(250, 128, 128));

    //Create a reflective surface
    Sphere sphereTwo(Vector3(100, 256, 450), 75, Vector3(40, 20, 20), Vector3(250, 128, 128), true);

    //Create a refractive surface
    Sphere sphereThree(Vector3(75, 400, 250), 50, Vector3(40, 20, 20), Vector3(250, 128, 128), false, true);

    //Add objects to the object list
    pObjectList.push_back(&cubeRoom);
    pObjectList.push_back(&cube);
    pObjectList.push_back(&sphereOne);
    pObjectList.push_back(&sphereTwo);
    pObjectList.push_back(&sphereThree);

    //Apply transformations: translations to the first sphere
    sphereOne.Translate(Vector3(150, 150, 150));

    srand (time(NULL));

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

                    bool shadow = false;
                    if(hasIntersection)
                    {
                        if(pObjectList[minObjectIndex]->refraction)
                        {
                            Vector3 refNormal = normalMin;
                            Vector3 refIntPoint = intPointMin;
                            int minRefObjectIndex = minObjectIndex;
                            Vector3 refAmbient;
                            Vector3 refDiffuse;
                            bool reflection = Reflection(direction, &refNormal, &refIntPoint, &minRefObjectIndex, &refAmbient, &refDiffuse);

                            Vector3 refractedDir;
                            float c;
                            Vector3 k;
                            float materialIndex = 1.55f;

                            if(math.DotProduct(direction, normalMin) < 0)
                            {
                                Refraction(direction, normalMin, materialIndex, &refractedDir);
                                Vector3 temp = math.MultiplyScalar(direction, -1.0f);
                                c = math.DotProduct(temp, normalMin);
                                k.x = k.y = k.z = 1;
                            }
                            else
                            {
                                //Don't know where these values come in
                                //k.x = exp(-a.x * t)
                                //k.y = exp(-a.y * t)
                                //k.z = exp(-a.z * t)
                                k.x = k.y = k.z = 0.2;
                                Vector3 negNormal = math.MultiplyScalar(normalMin, -1.0f);
                                bool refract = Refraction(direction, negNormal, 1.0f/materialIndex, &refractedDir);
                                if(refract)
                                {
                                    c = math.DotProduct(refractedDir, normalMin);
                                }
                                else
                                {
                                    //color = k * color(p + tr)
                                    refAmbient.x *= k.x;
                                    refAmbient.y *= k.y;
                                    refAmbient.z *= k.z;

                                    refDiffuse.x *= k.x;
                                    refDiffuse.y *= k.y;
                                    refDiffuse.z *= k.z;
                                }
                            }
                            float R_0 = ((materialIndex - 1) * (materialIndex - 1))/((materialIndex + 1) * (materialIndex + 1));
                            float R = R_0 + (1 - R_0) * pow((1 - c), 5.0f);

                            //Find the refraction colour
                            float minRefractT = 99999;
                            Vector3 minRefractNormal;
                            Vector3 minRefractIntPoint;
                            int minRefractObjectIndex;
                            Vector3 refractAmbient;
                            Vector3 refractDiffuse;
                            bool refraction;

                            for(int k = 0; k < pObjectList.size(); k++) {
                                float refractT;
                                Vector3 refractNormal;
                                Vector3 refractIntPoint;

                                //Shoot a ray in the object direction
                                refraction = pObjectList[k]->Intersect(camera, direction, &refractT, &refractNormal, &refractIntPoint);

                                //If it intersects, determine the colour
                                if(refraction)
                                {
                                    if(minRefractT > refractT)
                                    {
                                        minRefractT = refractT;
                                        minRefractNormal = refractNormal;
                                        minRefractIntPoint = refractIntPoint;
                                        minRefractObjectIndex = k;
                                        refractAmbient = pObjectList[k]->ambientColour;
                                        refractDiffuse = pObjectList[k]->diffuseColour;
                                    }
                                }
                            }
                            //color = k * (R * color(p + tr) + (1 - R)color(p + tt))

                            //Determine if either the relection point or the refraction point is in shadow
                            bool shadowOnReflect = false;
                            if(reflection)
                            {
                                shadowOnReflect = CheckShadow(refIntPoint, minRefObjectIndex);
                            }

                            bool shadowOnRefract = false;
                            if(refraction)
                            {
                                refraction = CheckShadow(minRefractIntPoint, minRefractObjectIndex);
                            }

                            refAmbient = math.MultiplyScalar(refAmbient, R);
                            refractAmbient = math.MultiplyScalar(refractAmbient, (1.0f - R));

                            refDiffuse = math.MultiplyScalar(refDiffuse, R);
                            refractDiffuse = math.MultiplyScalar(refractDiffuse, (1.0f - R));

                            if(shadowOnReflect && shadowOnRefract)
                            {
                                ambientColour = math.Add(refAmbient, refractAmbient);
                                diffuseColour = math.Add(refDiffuse, refractDiffuse);
                            }
                            else if(shadowOnReflect)
                            {
                                ambientColour = refAmbient;
                                diffuseColour = refDiffuse;
                            }
                            else if(shadowOnRefract)
                            {
                                ambientColour = refractAmbient;
                                diffuseColour = refractDiffuse;
                            }
                            else
                            {
                                shadow = true;
                            }
                        }
                        else
                        {
                            //Determine if an object is reflective
                            if(pObjectList[minObjectIndex]->reflection)
                            {
                                hasIntersection = Reflection(direction, &normalMin, &intPointMin, &minObjectIndex, &ambientColour, &diffuseColour);
                            }

                            //Determine if a pixel is in shadow from any light
                            lightsForShading.clear();
                            shadow = CheckShadow(intPointMin, minObjectIndex);
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
            Vector3 result = math.VectorAverage(colours);

            //Set the colour of the pixel
            Pixel shade;
            SetColour(shade, result);

            //Render the pixel
            (*pImage)(i, j) = shade;
        }
    }
}
