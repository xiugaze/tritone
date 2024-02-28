/**
 * @file vec.c
 * @author Caleb Andreano (andreanoc@msoe.edu)
 * @class CPE2600-121
 * @brief 3-dimensional vector struct and related mathematical operations
 * 
 * 
 * Course: CPE2600-121
 * Assignment: Lab Wk 5
 * @date 2023-10-01
 */

#include "vec.h"
#include <stdio.h>
#include <float.h>

/**
 * @brief Adds two vectors together and returns their sum
 * 
 * @param a 
 * @param b 
 * @return vector 
 */
vector vec_add(vector a, vector b) {
    vector sum = { a.i + b.i, a.j + b.j, a.k + b.k } ;
    return sum;
}

/**
 * @brief Subtracts two vectors and returns their difference
 * 
 * @param a 
 * @param b 
 * @return vector 
 */
vector vec_sub(vector a, vector b) {
    vector diff = { a.i - b.i, a.j - b.j, a.k - b.k } ;
    return diff;
}

/**
 * @brief Multiplies two vectors element-wise
 * 
 * @param a 
 * @param b 
 * @return vector 
 */
vector vec_mul(vector a, vector b) {
    vector prod = { a.i * b.i, a.j * b.j, a.k * b.k } ;
    return prod;
}

/**
 * @brief Takes the dot product of two vectors
 * 
 * @param a 
 * @param b 
 * @return float 
 */
float vec_dot(vector a, vector b) {
    return (a.i * b.i) + (a.j * b.j) + (a.k * b.k);
}

/**
 * @brief Takes the cros product of two vectors
 * 
 * @param a 
 * @param b 
 * @return vector 
 */
vector vec_cross(vector a, vector b) {
    float i = (a.j * b.k)  - (a.k * b.j);
    float j = -((a.i * b.k)  - (a.k * b.i));
    float k = (a.i * b.j)  - (a.j * b.i);
    vector cross = {i, j, k};
    return cross;
}

/**
 * @brief Converts a vector to a formatted string
 * 
 * @param v 
 * @return char* 
 */
char* vector_to_string(vector v) {
    static char buffer[60];
    snprintf(buffer, 60, "{ i: %.2f, j: %.2f, k: %.2f }", v.i, v.j, v.k);
    return buffer;
}