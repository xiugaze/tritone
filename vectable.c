/**
 * @file vectable.c
 * @author Caleb Andreano (andreanoc@msoe.edu)
 * @class CPE2600-121
 * @brief Vector Hashtable
 * Supports insertion in O(1) average time and retrieval at O(1) average time.
 * Uses dbj2 hashing for indexing and handles collision using linear probing 
 * 
 * Course: CPE2600-121
 * Assignment: Lab Wk 7
 * @date 2023-10-17
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vectable.h"

static vectable* table;
static int INITIALIZED = 0;

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

/**
 * @brief Allocates space for the vectable with an initial capacity
 * 
 * @return vectable* 
 */
vectable* new_vectable(void) {
    vectable* v = (vectable*)malloc(sizeof(vectable));
    v->entries = (vt_entry*)calloc(INITIAL_CAPACITY, sizeof(vt_entry));
    v->size = 0;
    v->capacity = INITIAL_CAPACITY;
    
    return v;
}

/**
 * @brief Sets the file vectable variable to an empty vectable
 * 
 */
void vectable_init(void) {
    table = new_vectable();
    INITIALIZED = 1;
}

/**
 * @brief Frees a list of vt_entry 
 * 
 * @param e Pointer to first entry
 * @param capacity How many entries to free
 * @return int 
 */
static int free_entries(vt_entry* e, int capacity) {
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

/**
 * @brief Frees a vectable, includes its entry
 * 
 * @return int 
 */
int free_vectable() {
    int freed = free_entries(table->entries, table->capacity);
    free(table);
    return freed;
}

/**
 * @brief Empties the file vectable
 * 
 * @return int 
 */
int clear_vectable() {
    int freed = free_vectable();
    table = new_vectable();
    return freed;
}


/**
 * @brief Resizes the vectable to a capacity of new_size
 * 
 * @param new_size 
 */
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


/***
 * Returns the current load factor of the vectable
*/
static float load_factor() {
    return (float)table->size/(float)table->capacity;
}

/**
 * @brief Inserts a vector
 * 
 * @param key Name of variable
 * @param value Vector to store
 */
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

        // if the probe reaches the end, loop back around
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

/**
 * @brief Returns the some invariant containing the vector
 * 
 * @param v 
 * @return vt_option 
 */
vt_option some(vt_entry v) {
    vt_option s;
    s.state = SOME;
    s.value = v;

    return s;
};

/**
 * @brief returns the none invariant
 * 
 * @return vt_option 
 */
vt_option none() {
    vt_option n;
    n.state = NONE;
    
    return n;
};

/**
 * @brief Returns true if o is some
 * 
 * @param o 
 * @return int 
 */
int is_some(vt_option o) {
    return o.state == SOME;
}

/**
 * @brief Returns some(vec) if the vector with name key exists, 
 * otherwise returns none
 * 
 * @param key 
 * @return vt_option 
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

/**
 * @brief Lists the variables in the vectable and summarizes its properties
 * 
 */
void print_vectable() {
    int found = 0;
    for(int i = 0; i < table->capacity; i++) {
        if(table->entries[i].key != NULL) {
            printf(
                "%s: %s\n", 
                table->entries[i].key, 
                vector_to_string(table->entries[i].value));
            found++;
        }
    }

    if(found == 0) {
        printf("No vectors are currently stored\n");
    } else {
        printf(
            "Summary: %d stored vectors at a %0.4f load factor\n", 
            table->size, 
            load_factor()
            );
    }
}

/**
 * @brief Writes the current vectable as a csv to path
 * 
 * @param path 
 */
void write_vectable(char* path) {
    FILE* fp = fopen(path, "w+");
    for(int i = 0; i < table->capacity; i++) {
        if(table->entries[i].key != NULL) {
            vt_entry* e = table->entries;
            fprintf(
                fp, 
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

/**
 * @brief Attempts to read a vectable from path
 * 
 * @param path 
 * @return int 
 */
int read_vectable(char* path) {
    FILE* fp = fopen(path, "r+");
    if(!fp) { return -1; }

    char name[40];
    float i;
    float j;
    float k;

    int scan_successes;
    int line = 1;
    int read = 0;
    while((scan_successes = fscanf(
        fp, 
        "%[^,],%f,%f,%f\n", 
        name, &i, &j, &k)
        ) != EOF) {
        if(scan_successes != 4) {
            printf("Error: Bad line at line %d\n, ignoring", line);
        } else {
            vector v = {i, j, k};
            insert_vector(name, v);
            read++;
        }
        line++;
    };
    fclose(fp);
    return read;
};

/**
 * @brief Generates a random string of length size
 * 
 * stolen from 
 * https://codereview.stackexchange.com/questions/29198/random-string-generator-in-c
*/
static char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyz"
                           "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

/**
 * @brief Inserts size random vectors into the table
 *  Note: Failure seems to be machine dependent:
 *  - Tested on Native Linux, Ryzen 5900X/32GB: breaks past ~700000 entries
 *  - Tested on WSL Linux, School Laptops: breaks past ~1500 entries
 * @param size 
 */
void fill_vectable(int size) {
    for (int i = 0; i < size; i++) {
        char* str = malloc(sizeof(char) * 13);
        str = rand_string(str, 12);
        vector v = { i, i, i };
        insert_vector(str, v);
        free(str);
    }
}