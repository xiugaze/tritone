#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vectable.h"


/**
 * @brief implementation of djb2 string hashing
 *        http://www.cse.yorku.ca/~oz/hash.html 
 * @param str 
 * @return long 
 */
int hash(char *key, int capacity) {
    unsigned long hash = 5381;
    int c;

    while (c = *key++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % capacity;
}

vectable* new_vectable(void) {
    vectable* v = (vectable*)malloc(sizeof(vectable));
    v->entries = (vt_entry*)calloc(INITIAL_CAPACITY, sizeof(vt_entry));
    v->size = 0;
    v->capacity = INITIAL_CAPACITY;
    
    return v;
}

void free_entries(vt_entry* e, int capacity) {
    for(int i = 0; i < capacity; i++) {
        free(e[i].key);
    }
    free(e);
}

void free_vectable(vectable* v) {
    free_entries(v->entries, v->capacity);
    free(v);
}


void resize_vectable(vectable* v, int new_size) {
    vt_entry* new_entries = (vt_entry*)calloc(new_size, sizeof(vt_entry));
    for(int i = 0; i < v->capacity; i++) {
        // for each existing entry in the old table
        if(v->entries[i].key != NULL) {
            int index = hash(v->entries[i].key, new_size);
            while(new_entries[index].key != NULL) {
                index++;
            }

            new_entries[index].key = malloc(strlen(v->entries[i].key) + 1);
            strcpy(new_entries[index].key, v->entries[i].key);
            new_entries[index].value = v->entries[i].value; 
        }
    }
    free_entries(v->entries, v->capacity);
    v->capacity = new_size;
    v->entries = new_entries;
}


static float load_factor(vectable* c) {
    return (float)c->size/(float)c->capacity;
}

void insert(vectable* v, char* key, vector value) {
    // check for load factor
    if(load_factor(v) >= 0.7) {
        printf("resizing to %d at key %s\n", v->capacity*2, key);
        resize_vectable(v, v->capacity*2);
    }

    // linear probe
    int index = hash(key, v->capacity);
    while(v->entries[index].key != NULL) {

        // if the key already exists
        if(!strcmp(v->entries[index].key, key)) {
            v->entries[index].value = value;
            return;
        }

        // if the probe reaches the end;
        if(index == v->capacity - 1) {
            index = 0;
        } else {
            index++;
        }
    }

    v->size++;
    v->entries[index].key = malloc(strlen(key) + 1);
    strcpy(v->entries[index].key, key);
    v->entries[index].value = value;
    
}

vt_option some(vector v) {
    vt_option s;
    s.state = SOME;
    s.value = v;

    return s;
};

vt_option none() {
    vt_option n;
    n.state = NONE;
    
    return n;
};

int is_some(vt_option o) {
    return o.state == SOME;
}

vt_option get(vectable* v, char* key) {
    int index = hash(key, v->capacity);

    // probe: existing keys are all in order
    while(v->entries[index].key != NULL) {

        if(!strcmp(v->entries[index].key, key)) {
            return some(v->entries[index].value);
        }

        // if we reach the end and haven't found it
        if(index == v->capacity - 1) {
            index = 0;
        } else {
            index++;
        }
    }
    return none();
}

void print_vectable(vectable* v) {
    for(int i = 0; i < v->capacity; i++) {
        if(v->entries[i].key != NULL) {
            printf("%s at index %d: %s\n", v->entries[i].key, i, vector_to_string(v->entries[i].value));
        }
    }
}