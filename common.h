#pragma once
#include <vector>
#include <array>

/**
 * Defines a 3D point or vector.
 */
struct Vector3 {
    /** Elements of the Vector3 struct. */
    float x, y, z;

    /** Default constructor. */
    Vector3()
    {
        x = 0;
        y = 0;
        z = 0;
    }

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

struct Matrix4x4 {
    float x1, x2, x3, x4;
    float y1, y2, y3, y4;
    float z1, z2, z3, z4;
    float w1, w2, w3, w4;
    Matrix4x4() {
        x1 = x2 = x3 = x4 = 0;
        y1 = y2 = y3 = y4 = 0;
        z1 = z2 = z3 = z4 = 0;
        w1 = w2 = w3 = w4 = 0;
    }
    Matrix4x4(float x1_in, float x2_in, float x3_in, float x4_in,
           float y1_in, float y2_in, float y3_in, float y4_in,
           float z1_in, float z2_in, float z3_in, float z4_in,
           float w1_in, float w2_in, float w3_in, float w4_in)
    {
        x1 = x1_in;
        x2 = x2_in;
        x3 = x3_in;
        x4 = x4_in;

        y1 = y1_in;
        y2 = y2_in;
        y3 = y3_in;
        y4 = y4_in;

        z1 = z1_in;
        z2 = z2_in;
        z3 = z3_in;
        z4 = z4_in;

        w1 = w1_in;
        w2 = w2_in;
        w3 = w3_in;
        w4 = w4_in;
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
    Vector3 VectorAverage(std::vector<Vector3> vList);

    Matrix4x4 GetRotationMatrixForAxis(Vector3 axis, float angle);
};
