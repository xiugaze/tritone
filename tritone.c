#include <stdio.h>
#include <string.h>
#include "tritone.h"
#include "ast.h"
#include "vec.h"
#include "vecvec.h"

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


/**
 * @brief Prints the help text
 * 
 */
void print_help() {
    printf("tritone: very bad vector calculator\n" 
           "- store a vector: a = 1, 2, 3\n"
           "- scalar operations: 1+2, 6-9, 5*3, 9/1,\n"
           "- vector operations: a + b, a + (1, 2, 3 * c)\n" 
           "\t-supports addition, subtraction, scalar multiplication," 
           " scalar division, cross product, dot product.\n"
           " help: print this message\n"
           " clear: clear the screen\n"
           " free: free all variables\n"
           " list: list all variables\n"
           );
}