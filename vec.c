#include "vec.h"
#include "vecvec.h"
#include <stdio.h>
#include <float.h>

/**
 * @brief Adds two vectors together
 * 
 * @param a 
 * @param b 
 * @return vector 
 */
vector vec_add(vector a, vector b) {
    vector sum = { a.i + b.i, a.j + b.j, a.k + b.k } ;
    return sum;
}

vector vec_sub(vector a, vector b) {
    vector diff = { a.i - b.i, a.j - b.j, a.k - b.k } ;
    return diff;
}

vector vec_mul(vector a, vector b) {
    vector prod = { a.i * b.i, a.j * b.j, a.k * b.k } ;
    return prod;
}

float vec_dot(vector a, vector b) {
    return (a.i * b.i) + (a.j * b.j) + (a.k * b.k);
}

vector vec_cross(vector a, vector b) {
    float i = (a.j * b.k)  - (a.k * b.j);
    float j = -((a.i * b.k)  - (a.k * b.i));
    float k = (a.i * b.j)  - (a.j * b.i);
    vector cross = {i, j, k};
    return cross;
}

char* vector_to_string(vector v) {
    static char buffer[60];
    snprintf(buffer, 60, "{ i: %.2f, j: %.2f, k: %.2f }", v.i, v.j, v.k);
    return buffer;
}

int is_max(vector a) {
    return (a.i == FLT_MAX) && (a.j == FLT_MAX) && (a.k == FLT_MAX);
}

vector vec_max(void) {
    vector m = {FLT_MAX, FLT_MAX, FLT_MAX};
    return m;
}