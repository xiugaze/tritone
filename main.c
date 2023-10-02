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
#include "vec.h"
#include "vecvec.h"
#include "ast.h"

char* tritone(void) {

    static int started = 0;
    if(!started) {
        printf("\033[0;35m");
        printf(" ____  ____  ____  ____  _____  _  _  ____    |\\\n");
        printf("(_  _)(  _ \\(_  _)(_  _)(  _  )( \\( )( ___)   |/\n");
        printf("  )(   )   / _)(_   )(   )(_)(  )  (  )__)   /|\n");
        printf(" (__) (_)\\_)(____) (__) (_____)(_)\\_)(____) ('|)\n");
        printf("  type 'help' for help                       \"| \n");
        printf("\n\033[0m");
        started = 1;

    }
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
