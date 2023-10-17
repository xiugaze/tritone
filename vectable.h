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
        vt_entry value;
        vt_result_state state;
    } vt_option;

    vectable* new_vectable(void);
    void free_vectable();
    void resize_vectable(int new_size);
    void insert_vector(char* key, vector value);
    void print_vectable();
    int is_some(vt_option o);
    vt_option get_vector(char* key);
    void write_vectable();
    int read_vectable();
    void vectable_init();

#endif