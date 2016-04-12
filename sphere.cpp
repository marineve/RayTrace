#include "Sphere.h"

Sphere::Sphere(Vector3 centerIn, float radiusIn, Vector3 ambient, Vector3 diffuse, bool reflection, bool refraction) :
    Object(ambient, diffuse, reflection, refraction),
    center(centerIn),
    radius(radiusIn)
{}

bool Sphere::Intersect(Vector3 origin, Vector3 direction,
                  float *tOut, Vector3 *normalOut, Vector3* intPoint, bool backPoint,
				  Vector3 *aColour, Vector3 *dColour)
{
    Vector3 EO = math.Minus(center, origin);
    float v = math.DotProduct(EO, direction);
    float radiusSquared = radius * radius;
    float EOSquared = math.DotProduct(EO, EO);
    float discriminant = radiusSquared - (EOSquared - v * v);

    float t = -1;
    if(discriminant > 0)
    {
        float d = sqrt(discriminant);
        t = backPoint ? v + d : v - d;
    }

    if(t > 0)
    {
        *tOut = t;
        Vector3 intersectionPoint = math.MultiplyScalar(direction,t);
        intersectionPoint = math.Add(intersectionPoint, origin);
        *intPoint = intersectionPoint;
        Vector3 surfaceNormal = math.Minus(intersectionPoint, center);
        (*normalOut) = math.Normalize(surfaceNormal);
        return true;
    }
    else
    {
        return false;
    }
}

void Sphere::Translate(Vector3 difference)
{
    v[0] = center.x;
    v[1] = center.y;
    v[2] = center.z;
    v[3] = 1;

    //Initialize the transformation matrix
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(i == j)
            {
                m[i][j] = 1;
            }
            else
            {
                m[i][j] = 0;
            }
        }
    }

    m[3][0] = difference.x;
    m[3][1] = difference.y;
    m[3][2] = difference.z;

    //Apply the transformation
    Vector3 result = Transform();
    center = result;
}

Vector3 Sphere::Transform()
{
    Vector3 result;
    //Calculate the result of the matrix
    result.x = v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m[2][0] + v[3] * m[3][0];
    result.y = v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m[2][1] + v[3] * m[3][1];
    result.z = v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m[2][2] + v[3] * m[3][2];
    int w = v[0] * m[0][3] + v[1] * m[1][3] + v[2] * m[2][3] + v[3] * m[3][3];

    return result;
}

std::vector < Vector3 > Sphere::GetBoundaryPoints() {
	Vector3 LLF(center.x - radius, center.y - radius, center.z - radius);
	Vector3 LRF(center.x + radius, center.y - radius, center.z - radius);
	Vector3 ULF(center.x - radius, center.y + radius, center.z - radius);
	Vector3 URF(center.x + radius, center.y + radius, center.z - radius);
	Vector3 LLB(center.x - radius, center.y - radius, center.z + radius);
	Vector3 LRB(center.x + radius, center.y - radius, center.z + radius);
	Vector3 ULB(center.x - radius, center.y + radius, center.z + radius);
	Vector3 URB(center.x + radius, center.y + radius, center.z + radius);

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