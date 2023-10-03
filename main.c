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


int main(int arc, char** argv) {
    if(argv[1] && !strcmp("-h", argv[1])) {
        print_help();
        exit(0);
    }

    do {
        printf("%s", tritone());
    } while(1);

    return -1;
}
