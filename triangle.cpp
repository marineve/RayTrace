#include "triangle.h"

Triangle::Triangle(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 ambient, Vector3 diffuse, bool reflection) :
    Object(ambient, diffuse, reflection),
    point1(p1),
    point2(p2),
    point3(p3)
{
    //Define constants for use in calculations
    a = point1.x - point2.x;
    b = point1.y - point2.y;
    c = point1.z - point2.z;
    d = point1.x - point3.x;
    e = point1.y - point3.y;
    f = point1.z - point3.z;
}

bool Triangle::Intersect(Vector3 origin, Vector3 direction,
                           float* tOut, Vector3* normalOut, Vector3* intPoint)
{
    //Define the rest of the constants
    g = direction.x;
    h = direction.y;
    i = direction.z;
    j = point1.x - origin.x;
    k = point1.y - origin.y;
    l = point1.z - origin.z;

    float ei_minus_hf = e * i - h * f;
    float gf_minus_di = g * f - d * i;
    float dh_minus_eg = d * h - e * g;
    float ak_minus_jb = a * k - j * b;
    float jc_minus_al = j * c - a * l;
    float bl_minus_kc = b * l - k * c;

    //Compute denominator M
    float M = a * (ei_minus_hf) + b * (gf_minus_di) + c * (dh_minus_eg);

    //Compute t
    float t = -1 * (f * (ak_minus_jb) + e * (jc_minus_al) + d * (bl_minus_kc))/M;
    if(t < 0)
        return false;

    //Compute gamma
    float gamma = (i * (ak_minus_jb) + h * (jc_minus_al) + g * (bl_minus_kc))/M;
    if(gamma < 0 || gamma > 1)
        return false;

    //Compute beta
    float beta = (j * (ei_minus_hf) + k * (gf_minus_di) + l * (dh_minus_eg))/M;
    if(beta < 0 || beta > 1 - gamma)
        return false;

    *tOut = t;

    //Find the point of intersection
    Vector3 intersectionPoint = math.MultiplyScalar(direction,t);
    intersectionPoint = math.Add(intersectionPoint, origin);
    *intPoint = intersectionPoint;

    //Find the normal vector
    Vector3 normal = math.CrossProduct(math.Minus(point1, point2), math.Minus(point1, point3));
    *normalOut = math.Normalize(normal);

    return true;
}
