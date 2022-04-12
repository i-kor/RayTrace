#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "vec.h"

// Creates a new vector
struct Vec Vec (double x, double y, double z)
{
    struct Vec ret = {x, y, z};
    return ret;
}

// Creates an inverted vector
struct Vec VecInv (const struct Vec vec)
{
    return Vec(-vec.e[0],
                -vec.e[1],
                -vec.e[2]);
}

// Multiplies vector by number
struct Vec VecIncr (struct Vec left, const double right)
{
    return Vec(left.e[0]*right,
                left.e[1]*right,
                left.e[2]*right);
}

// Divides vector by number
struct Vec VecDecr (struct Vec left, const double right)
{
    return Vec(left.e[0]/right,
                left.e[1]/right,
                left.e[2]/right);
}

// Returns a lenght of a vector
double VecLen (const struct Vec vec)
{
    return sqrt(VecLenSq(vec));
}

// Returns a squared lenght of a vector
double VecLenSq (const struct Vec vec)
{
    return vec.e[0]*vec.e[0] +
           vec.e[1]*vec.e[1] +
           vec.e[2]*vec.e[2];
}

// Adds two vectors
struct Vec VecAdd (const struct Vec left, const struct Vec right)
{
    return Vec(left.e[0]+right.e[0],
                left.e[1]+right.e[1],
                left.e[2]+right.e[2]);
}

// Substracts two vectors
struct Vec VecSub (const struct Vec left, const struct Vec right)
{
    return Vec(left.e[0]-right.e[0],
                left.e[1]-right.e[1],
                left.e[2]-right.e[2]);
}

// Multiplies two vectors
struct Vec VecMult (const struct Vec left, const struct Vec right)
{
    return Vec(left.e[0]*right.e[0],
                left.e[1]*right.e[1],
                left.e[2]*right.e[2]);
}

// Dot product of vectors
double VecDot (const struct Vec left, const struct Vec right)
{
    return left.e[0]*right.e[0] +
           left.e[1]*right.e[1] +
           left.e[2]*right.e[2];
}

// Cross product of vectors
struct Vec VecCross (const struct Vec left, const struct Vec right)
{
    return Vec(left.e[1] * right.e[2] - left.e[2] * right.e[1],
                left.e[2] * right.e[0] - left.e[0] * right.e[2],
                left.e[0] * right.e[1] - left.e[1] * right.e[0]);
}

// Return a vec of lenght 1
struct Vec VecUnit (const struct Vec left)
{
    return VecDecr(left, VecLen(left));
}

// Prints a vector to a string
char* VecPrn(struct Vec vec)
{
    int len = snprintf(NULL, 0, "%f %f %f", vec.e[0], vec.e[1], vec.e[2]);
    char *ret = malloc(sizeof(char) * (len + 1));
    snprintf(ret, len, "%.2f %.2f %.2f", vec.e[0], vec.e[1], vec.e[2]);
    return ret;
}
