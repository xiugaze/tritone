/**
 * @file ast.c
 * @author Caleb Andreano (andreanoc@msoe.edu)
 * @class CPE2600-121
 * @brief Array of vectors. Supports insertion and 
 * retreival, and full clearing in O(n) time, and freeing
 * a single vector in O(1) time. 
 * 
 * 
 * Course: CPE2600-121
 * Assignment: Lab Wk 5
 * @date 2023-10-01
 */

#include "vecvec.h"
#include "vec.h"
#include <stdio.h>
#include <string.h>

static vec_cell vectors[MAX_VECS];
static int INITIALIZED = 0;

/**
 * @brief Marks each vector in the vector struct 
 * as open. 
 * 
 * @return int 
 */
int clear_vectors() {
    int cleared = 0;
    for(int i = 0; i < 10; i++) {
        if(vectors[i].state == CLOSED) {
            vectors[i].state = OPEN;
            cleared++;
        }
    }
    return cleared;
}

/**
 * @brief Prints a list of all stored vector
 * 
 */
void list_vectors() {
    printf("Stored Vectors: \n");
    for(int i = 0; i < 10; i++) {
        if(vectors[i].state == CLOSED) {
            printf("\t%s: %s\n", vectors[i].name, vector_to_string(vectors[i].vec));
        }
    }
}

/**
 * @brief Inserts a vector into the vector array as a named variable
 * 
 * @param vec Vector to store
 * @param name Variable name
 * @return int 
 */
int insert_vector(vector vec, char* name) {
    if(!INITIALIZED) {
        clear_vectors();
        INITIALIZED = 1;
    }

    if (strlen(name) > 12) {
        printf("Error: Variable name must be 12 characters or less\n");
        return -1;
    }

    for (int i = 0; i < MAX_VECS; i++) {
        if(vectors[i].state == OPEN) {
            vectors[i].state = CLOSED;
            vectors[i].vec = vec;
            strcpy(vectors[i].name, name);
            return 0;
        } else if(!strcmp(vectors[i].name, name)) {
            vectors[i].vec = vec;
            strcpy(vectors[i].name, name);
            return 0;
        }
    }
    printf("Error: Vector storage is full. Please free a variable.\n");
    return -1;
}

/**
 * @brief Get a named vector pointer from the vector array
 * 
 * @param name Name of the vector 
 * @return vec_cell* 
 */
vec_cell* get_vector(char* name) {
    for(int i = 0; i < MAX_VECS; i++) {
        if(!strcmp(name, vectors[i].name) && vectors[i].state == CLOSED) {
            return &vectors[i];
        }
    }
    return NULL;
}

/**
 * @brief Marks a vector as free
 * 
 * @param name Name of vector to free
 * @return int 
 */
int free_vector(char* name) {
    printf("Freeing %s\n", name);
    for(int i = 0; i < MAX_VECS; i++) {
        if(!strcmp(name, vectors[i].name)) {
            vectors[i].state = OPEN;
            return 0;
        }
    }
    return -1;
}

/**
 * @brief Converts a vec_cell to a string
 * 
 * @param v 
 * @return char* 
 */
char* vec_cell_to_string(vec_cell* v) {
    static char buffer[80];
    snprintf(buffer, 80, "%s: %s", v->name, vector_to_string(v->vec));
    return buffer;
}
