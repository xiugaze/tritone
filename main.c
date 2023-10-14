/**
 * @file main.c
 * @author Caleb Andreano (andreanoc@msoe.edu)
 * @class CPE2600-121
 * @brief Tritone: a bad vector calculator
 * Supports
 * 
 * 
 * Course: CPE2600-121
 * Assignment: Lab Wk 5
 * @date 2023-10-01
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tritone.h"
#include "vectable.h"


int main(int arc, char** argv) {

    // if(argv[1] && !strcmp("-h", argv[1])) {
    //     print_help();
    //     exit(0);
    // }

    // atexit(tritone_exit);

    // do {
    //     printf("%s", tritone());
    // } while(1);

    // return -1;
    vectable* v = new_vectable();

    vector a = { 1, 2, 3 };
    char c1 = 'A';
    char c2 = 'z';
    char buf[3];

    for(int i = 0; i < 32; i++) {
        sprintf(buf, "%c%c", c1, c2);
        insert(v, buf, a);
        c1++;
        c2--;
        a.i++;
        a.j++;
        a.k++;
    }

    // insert(v, "a", a);
    // insert(v, "nineteen", a);
    // resize_vectable(v, 32);
    print_vectable(v);
    printf("size: %d, capacity: %d\n", v->size, v->capacity);

    free_vectable(v);
    return 0;
}
