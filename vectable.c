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

    while ( (c = *key++) != 0)
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

static vectable* table;
static int INITIALIZED = 0;
void vectable_init(void) {
    table = new_vectable();
}

int free_entries(vt_entry* e, int capacity) {
    int freed = 0;
    for(int i = 0; i < capacity; i++) {
        if(e[i].key != NULL) {
            free(e[i].key);
            freed++;
        }
    }
    free(e);
    return freed;
}

int free_vectable() {
    int freed = free_entries(table->entries, table->capacity);
    free(table);
    return freed;
}


void resize_vectable(int new_size) {
    vt_entry* new_entries = (vt_entry*)calloc(new_size, sizeof(vt_entry));
    for(int i = 0; i < table->capacity; i++) {
        // for each existing entry in the old table
        if(table->entries[i].key != NULL) {
            int index = hash(table->entries[i].key, new_size);
            while(new_entries[index].key != NULL) {
                index++;
            }

            new_entries[index].key = malloc(strlen(table->entries[i].key) + 1);
            strcpy(new_entries[index].key, table->entries[i].key);
            new_entries[index].value = table->entries[i].value; 
        }
    }
    free_entries(table->entries, table->capacity);
    table->capacity = new_size;
    table->entries = new_entries;
}


static float load_factor() {
    return (float)table->size/(float)table->capacity;
}

void insert_vector(char* key, vector value) {
    // check for load factor
    if(!INITIALIZED) {
        vectable_init();
        INITIALIZED = 1;
    }
    if(load_factor(table) >= 0.7) {
        printf("resizing to %d at key %s\n", table->capacity*2, key);
        resize_vectable(table->capacity*2);
    }

    // linear probe
    int index = hash(key, table->capacity);
    while(table->entries[index].key != NULL) {

        // if the key already exists
        if(!strcmp(table->entries[index].key, key)) {
            table->entries[index].value = value;
            return;
        }

        // if the probe reaches the end;
        if(index == table->capacity - 1) {
            index = 0;
        } else {
            index++;
        }
    }

    table->size++;
    table->entries[index].key = malloc(strlen(key) + 1);
    strcpy(table->entries[index].key, key);
    table->entries[index].value = value;
    
}

vt_option some(vt_entry v) {
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

/* 
    TODO: get_vector returns the actual address of the entry, &vectors[i].
*/
vt_option get_vector(char* key) {
    int index = hash(key, table->capacity);

    // probe: existing keys are all in order
    while(table->entries[index].key != NULL) {

        if(!strcmp(table->entries[index].key, key)) {
            return some(table->entries[index]);
        }

        // if we reach the end and haven't found it
        if(index == table->capacity - 1) {
            index = 0;
        } else {
            index++;
        }
    }
    return none();
}

void print_vectable() {
    for(int i = 0; i < table->capacity; i++) {
        if(table->entries[i].key != NULL) {
            printf("%s at index %d: %s\n", table->entries[i].key, i, vector_to_string(table->entries[i].value));
        }
    }
}

void write_vectable() {
    FILE* fp = fopen("./table/table", "w+");
    for(int i = 0; i < table->capacity; i++) {
        if(table->entries[i].key != NULL) {
            vt_entry* e = table->entries;
            fprintf(fp, 
                    "%s,%.2lf,%.2lf,%.2lf\n",
                    e[i].key,
                    e[i].value.i,
                    e[i].value.j,
                    e[i].value.k
            );
        }
    }
    fclose(fp);
}

void read_vectable() {
    FILE* fp = fopen("./table/table", "r+");
    if(!fp) { return; }

    char name[40];
    float i;
    float j;
    float k;

    int scan_successes;
    int line = 1;
    while((scan_successes = fscanf(fp, "%[^,],%f,%f,%f\n", name, &i, &j, &k)) != EOF) {
        if(scan_successes != 4) {
            printf("Error: Bad line at line %d\n, ignoring", line);
        } else {
            printf("name: %s\n", name);
            vector v = {i, j, k};
            insert_vector(name, v);
        }
        line++;
    };
};