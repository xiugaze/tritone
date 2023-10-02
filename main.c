#include <stdio.h>
#include "vec.h"
#include "vecvec.h"
#include "ast.h"


int main(void) {
    // int exit = 0;
    // do  {

    // } while(!exit);
    // for(int i = 0; i < 10; i++) {
    //     char name[10];
    //     sprintf(name, "var%d", i);
    //     vector vec = { i, i, i};
    //     insert_vector(vec, name);
    // }

    // vector v = {2, 2, 2};
    // insert_vector(v, "bad");
    // free_vector("var2");
    // insert_vector(v, "bad");

    char buffer[300];
    do {
        printf("\033[0;35m");
        printf("tritone");
        printf("\033[0m");
        printf("> ");
        fgets(buffer, 300, stdin);
        node* root = parse_input(buffer);
//        print_ast(root);
        value result = evaluate_ast(root);
        printf("%s", value_to_string(result));
        free_ast(root);
    } while(1);
    return 0;
}
