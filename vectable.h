#ifndef VECTABLE_H
#define VECTABLE_H

    #include "vec.h"
    #define INITIAL_CAPACITY 16

    typedef struct {
        char* key;
        vector value;
    } vt_entry;

    typedef struct {
        vt_entry* entries;
        int size;           // how many entries
        int capacity;       // maximum number of entries
    } vectable;

    typedef enum {
        SOME,
        NONE,
    } vt_result_state;

    typedef struct {
        vector value;
        vt_result_state state;
    } vt_option;

    vectable* new_vectable(void);
    void free_vectable(vectable* v);
    void resize_vectable(vectable* v, int new_size);
    void insert(vectable* v, char* key, vector value);
    void print_vectable(vectable* v);
    int is_some(vt_option o);
    vt_option get(vectable* v, char* key);

#endif