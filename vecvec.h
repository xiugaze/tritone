#ifndef VECVEC_H
#define VECVEC_H

    #define MAX_VECS 10
    #include "vec.h"

    typedef enum {
        OPEN = 0, 
        CLOSED,
    } vec_state;

    typedef struct {
        vec_state state;
        char name[12];
        vector vec;
    } vec_cell;

    int clear_vectors();
    int insert_vector(vector vec, char name[]);
    vec_cell* get_vector(char* name);
    char* vec_cell_to_string(vec_cell* v);
    void list_vectors();

#endif