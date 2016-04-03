#include "cube.h"

Cube::Cube(CubeConstruct cube, Vector3 ambient, Vector3 diffuse, bool reflection) :
    Object(ambient, diffuse, reflection)
{
    //Iterate through the faces on the cube
    for(int i = 0; i < cube.faces.size(); i++)
    {
        Vector3 p1 = cube.vertices[cube.faces[i][0]];
        Vector3 p2 = cube.vertices[cube.faces[i][1]];
        Vector3 p3 = cube.vertices[cube.faces[i][2]];
        Vector3 p4 = cube.vertices[cube.faces[i][3]];

        //Create two triangles on every face
        Triangle t1(p1, p2, p3, ambient, diffuse, reflection);
        Triangle t2(p1, p3, p4, ambient, diffuse, reflection);

        triangles.push_back(t1);
        triangles.push_back(t2);
    }
}

bool Cube::Intersect(Vector3 origin, Vector3 direction,
               float* tOut, Vector3* normalOut, Vector3* intPoint)
{
    //Iterate through the triangles
    for(int i = 0; i < triangles.size(); i++)
    {
        //Call intersect on each individual triangle
        bool intersect = triangles[i].Intersect(origin, direction, tOut, normalOut, intPoint);
        if(intersect)
            return true;
    }
    return false;
}
