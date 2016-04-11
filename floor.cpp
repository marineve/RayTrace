#include "floor.h"
#include <stdio.h>
#include <iostream>

Floor::Floor(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 ambient, Vector3 diffuse) :
    Object(ambient, diffuse),
    point1(p1),
    point2(p2),
    point3(p3)
{
    //Find the normal
    normal = math.CrossProduct(point1, point2);
}

bool Floor::Intersect(Vector3 origin, Vector3 direction,
          float* tOut, Vector3* normalOut, Vector3* intPoint)
{
    Vector3 QE = math.Minus(point3, origin);

    float N_dot_QE = math.DotProduct(normal, QE);
    float N_dot_v = math.DotProduct(normal, direction);

    float t = N_dot_QE/N_dot_v;
    if(t >= 0)
    {
        //Find the intersection point
        Vector3 intersectionPoint = math.MultiplyScalar(direction, t);
        *intPoint = math.Add(origin, intersectionPoint);

        *tOut = t;
        *normalOut = math.Normalize(normal);
        return true;
    }
    return false;
}
