#include "vec.h"

typedef struct {
    char* key;
    vec* value;
} vt_cell;

typedef struct {
    h_cell* entries;
    int size;
    int capacity;
} vectable;

static vectable table;

int hash(char* key);

create_vt_cell(char* key, vec* value) {
    char* name = malloc(strlen(key));
    strcpy(name, key);
    return vt_cell {
        name;
        value;
    }
}

/**
 * @brief implementation of djb2 string hashing
 *        http://www.cse.yorku.ca/~oz/hash.html 
 * @param str 
 * @return long 
 */
int hash(char *key) {
    unsigned long hash = 5381;
    int c;

    while (c = *key++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

void insert(char* name, vec* value) {

    int index = hash(name) % table.size;

    // if out of bounds, resize and attempt an insert again
    if(index > table.size) {
        resize();
        insert(name, value);
        return;
    }

    while(table[index] != NULL) {
        index++;
    }

    table[index] = create_vt_cell(name, value);
}

vt_cell* get(char* key) {
    int index = hash(key) % table.size;
    while(!strcmp(table[index].key, key)) {
        index++;
    }
    return &table[index];
}