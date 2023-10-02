/**
 * @file ast.c
 * @author Caleb Andreano (andreanoc@msoe.edu)
 * @class CPE2600-121
 * @brief Tritone: 
 * 
 * 
 * Course: CPE2600-121
 * Assignment: Lab Wk 5
 * @date 2023-10-01
 */

#include <stdio.h>
#include <string.h>
#include "vec.h"
#include "vecvec.h"
#include "ast.h"

char* tritone(void) {

        static char input_buffer[300];
        static char output_buffer[300];

        printf("\033[0;35m");
        printf("tritone");
        printf("\033[0m");
        printf("> ");

        fgets(input_buffer, 300, stdin);
        node* root = parse_input(input_buffer);
        strncpy(output_buffer, value_to_string(evaluate_ast(root)), 300);

        free_ast(root);
        return output_buffer;

}

int main(void) {

    do {
        printf("%s", tritone());
    } while(1);

    return -1;
}
