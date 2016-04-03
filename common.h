#pragma once
#include <vector>
#include <array>

/**
 * Defines a 3D point or vector.
 */
struct Vector3
{
    /** Elements of the Vector3 struct. */
    float x, y, z;

    /** Default constructor. */
    Vector3() {}

    /**
     * Constructor.
     * @param xIn - x coordinate
     * @param yIn - y coordinate
     * @param zIn - z coordinate
     */
    Vector3(float xIn, float yIn, float zIn)
    {
        x = xIn;
        y = yIn;
        z = zIn;
    }
};

/**
 * Constructs the faces of a 3D cube.
 */
struct CubeConstruct
{
    /** The vertices of the cube. */
    std::vector<Vector3> vertices;
    /** The faces of the cube. */
    std::vector< std::vector<int> > faces;

    /** Default constructor. */
    CubeConstruct() {}
    CubeConstruct(std::vector<Vector3> verts)
    {
        vertices = verts;

        std::vector<int> temp1;
        temp1.push_back(0);
        temp1.push_back(1);
        temp1.push_back(2);
        temp1.push_back(3);

        std::vector<int> temp2;
        temp2.push_back(1);
        temp2.push_back(4);
        temp2.push_back(7);
        temp2.push_back(2);

        std::vector<int> temp3;
        temp3.push_back(4);
        temp3.push_back(5);
        temp3.push_back(6);
        temp3.push_back(7);

        std::vector<int> temp4;
        temp4.push_back(5);
        temp4.push_back(0);
        temp4.push_back(3);
        temp4.push_back(6);

        std::vector<int> temp5;
        temp5.push_back(5);
        temp5.push_back(4);
        temp5.push_back(1);
        temp5.push_back(0);

        std::vector<int> temp6;
        temp6.push_back(3);
        temp6.push_back(2);
        temp6.push_back(7);
        temp6.push_back(6);

        faces.push_back(temp1);
        faces.push_back(temp2);
        faces.push_back(temp3);
        faces.push_back(temp4);
        faces.push_back(temp5);
        faces.push_back(temp6);
    }
};

/**
 * Definition of the Common class
 */
class Common
{
public:
    /** Common class constructor. */
    Common();

    /**
     * Adds two 3D vectors together.
     *
     * @param a
     * @param b
     * @return the resulting vector
     */
    Vector3 Add(Vector3 a, Vector3 b);

    /**
     * Subracts two 3D vectors.
     *
     * @param a
     * @param b
     * @return the resulting vector from b to a
     */
    Vector3 Minus(Vector3 a, Vector3 b);

    /**
     * Multiplies the elements of a vector by a numeric value.
     *
     * @param a - a vector
     * @param b - a numeric value
     * @return the resulting vector
     */
    Vector3 MultiplyScalar(Vector3 a, float b);

    /**
     * Finds the dot product of two vectors.
     *
     * @param a
     * @param b
     * @return the resulting numeric value
     */
    float DotProduct(Vector3 a, Vector3 b);

    /**
     * Normalizes a given vector.
     *
     * @param a
     * @return the normalized vector
     */
    Vector3 Normalize(Vector3 a);

    /**
     * Computes the cross product of two vectors.
     *
     * @param a
     * @param b
     * @return the resulting vector
     */
    Vector3 CrossProduct(Vector3 a, Vector3 b);

    /**
     * Determines the length of a vector.
     *
     * @param a
     * @return the length of the vector
     */
    float VectorLength(Vector3 a);

    /**
     * Finds the average of four vectors.
     *
     * @param a
     * @param b
     * @param c
     * @param d
     * @return a vector containing the resulting averaged values
     */
    Vector3 VectorAverage(Vector3 a, Vector3 b, Vector3 c, Vector3 d);
};
