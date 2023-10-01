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
        printf("tritone> ");
        fgets(buffer, 300, stdin);
        node* root = parse_input(buffer);
        vector result = evaluate_ast(root);
        if(!is_max(result)) {
            printf("%s\n", vector_to_string(result));
        }
        free_ast(root);
    } while(1);
    return 0;
}
