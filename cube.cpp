#include "cube.h"

Cube::Cube(CubeConstruct cube, Vector3 ambient, Vector3 diffuse, bool reflection, bool refraction, bool normalDirOut) :
    Object(ambient, diffuse, reflection, refraction)
{
    //Iterate through the faces on the cube
    for(int i = 0; i < cube.faces.size(); i++)
    {
        Vector3 p1 = cube.vertices[cube.faces[i][0]];
        Vector3 p2 = cube.vertices[cube.faces[i][1]];
        Vector3 p3 = cube.vertices[cube.faces[i][2]];
        Vector3 p4 = cube.vertices[cube.faces[i][3]];

        //Create two triangles on every face
        Triangle t1(p1, p2, p3, ambient, diffuse, reflection, normalDirOut);
        Triangle t2(p1, p3, p4, ambient, diffuse, reflection, normalDirOut);

        triangles.push_back(t1);
        triangles.push_back(t2);
    }

	//Define the boundaries
	boundaryPoints.push_back(cube.vertices[0]);
	boundaryPoints.push_back(cube.vertices[1]);
	boundaryPoints.push_back(cube.vertices[3]);
	boundaryPoints.push_back(cube.vertices[5]);
}

bool Cube::Intersect(Vector3 origin, Vector3 direction,
               float* tOut, Vector3* normalOut, Vector3* intPointOut, bool backPoint,
			   Vector3 *aColour, Vector3 *dColour)
{
    float tMin = 999999;
    Vector3 normalMin;
    Vector3 intPointMin;
    bool hasIntersection = false;

    //Iterate through the triangles
    for(int i = 0; i < triangles.size(); i++)
    {
        float t;
        Vector3 normal;
        Vector3 intPoint;

        //Call intersect on each individual triangle
        bool intersect = triangles[i].Intersect(origin, direction, &t, &normal, &intPoint);

        //If it intersects, determine the colour
        if(intersect) {
            hasIntersection = true;
            if(tMin > t) {
                tMin = t;
                normalMin = normal;
                intPointMin = intPoint;
            }
        }
    }

    *tOut = tMin;
    *normalOut = normalMin;
    *intPointOut = intPointMin;

    if(hasIntersection) {
        return true;
    } else {
        return false;
    }
}

std::vector < Vector3 > Cube::GetBoundaryPoints() 
{
	Vector3 LLF(boundaryPoints[0].x, boundaryPoints[0].y, boundaryPoints[0].z);
	Vector3 LRF(boundaryPoints[0].x, boundaryPoints[2].y, boundaryPoints[0].z);
	Vector3 ULF(boundaryPoints[1].x, boundaryPoints[0].y, boundaryPoints[0].z);
	Vector3 URF(boundaryPoints[1].x, boundaryPoints[2].y, boundaryPoints[0].z);
	Vector3 LLB(boundaryPoints[0].x, boundaryPoints[0].y, boundaryPoints[3].z);
	Vector3 LRB(boundaryPoints[0].x, boundaryPoints[2].y, boundaryPoints[3].z);
	Vector3 ULB(boundaryPoints[1].x, boundaryPoints[0].y, boundaryPoints[3].z);
	Vector3 URB(boundaryPoints[1].x, boundaryPoints[2].y, boundaryPoints[3].z);

	std::vector < Vector3 > result;
	result.push_back(LLF);
	result.push_back(LRF);
	result.push_back(ULF);
	result.push_back(URF);
	result.push_back(LLB);
	result.push_back(LRB);
	result.push_back(ULB);
	result.push_back(URB);

	return result;
}
