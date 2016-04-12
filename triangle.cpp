#include "triangle.h"

Triangle::Triangle(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 ambient, Vector3 diffuse, bool reflection, bool refraction, bool normalDirOut) :
Object(ambient, diffuse, reflection, refraction),
point1(p1),
point2(p2),
point3(p3),
normDirOut(normalDirOut)
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
	float* tOut, Vector3* normalOut, Vector3* intPoint, bool backPoint,
	Vector3 *aColour, Vector3 *dColour)
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
	float M = a * (ei_minus_hf)+b * (gf_minus_di)+c * (dh_minus_eg);

	//Compute t
	float t = -1 * (f * (ak_minus_jb)+e * (jc_minus_al)+d * (bl_minus_kc)) / M;
	if (t < 0)
		return false;

	//Compute gamma
	float gamma = (i * (ak_minus_jb)+h * (jc_minus_al)+g * (bl_minus_kc)) / M;
	if (gamma < 0 || gamma > 1)
		return false;

	//Compute beta
	float beta = (j * (ei_minus_hf)+k * (gf_minus_di)+l * (dh_minus_eg)) / M;
	if (beta < 0 || beta > 1 - gamma)
		return false;

	*tOut = t;

	//Find the point of intersection
	Vector3 intersectionPoint = math.MultiplyScalar(direction, t);
	intersectionPoint = math.Add(intersectionPoint, origin);
	*intPoint = intersectionPoint;

	//Find the normal vector
	/*Vector3 vec1 = normDirOut ? math.Minus(point3, point2) : math.Minus(point1, point2);
	Vector3 vec2 = normDirOut ? math.Minus(point3, point1) : math.Minus(point1, point3);
	Vector3 normal = math.CrossProduct(vec1, vec2);*/
	Vector3 normal = math.Normalize(math.CrossProduct(math.Minus(point1, point2), math.Minus(point1, point3)));
	//Check the angle that the normal makes with the incoming ray
	if (point1.x == 425 && point1.y == 425 && point1.z == 50 && direction.z > 0)
	{
		int x = 1;
	}

	float normal_dot_ray = math.DotProduct(normal, direction);
	if (normal_dot_ray > 0)
	{
		normal = math.MultiplyScalar(normal, -1);
	}

    *normalOut = normal;

    return true;
}
