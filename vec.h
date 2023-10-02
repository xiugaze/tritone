#ifndef VEC_H
#define VEC_H 

    typedef struct {
        float i;
        float j;
        float k;
    } vector;


    vector vec_add(vector a, vector b);
    vector vec_sub(vector a, vector b);
    vector vec_mul(vector a, vector b);
    float vec_dot(vector a, vector b);
    vector vec_cross(vector a, vector b);
    char* vector_to_string(vector v);
    vector vec_max(void);
    int is_max(vector a);
    int free_vector(char* name);

#endif