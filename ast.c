/**
 * @file ast.c
 * @author Caleb Andreano (andreanoc@msoe.edu)
 * @class CPE2600-121
 * @brief Helper routines for parsing an input string, constructing
 * an abstract syntax tree according to context free grammar G,
 * and evaluating the tree to a certain result.
 * 
 * Let G := 
 *  1. <statement> := <statement><statement> 
 *  2. <statement> := <assignment> | <expression>
 *  3. <assignment> := <identifier> = <expression> 
 *  4. <expression> := <term> | <term> {+|-} <expression> 
 *  5. <term> := <factor> | <factor> {*|/} <term>
 *  6. <factor> := <identifier> | <vector> | <constant> |(<expression>)
 *  7. <identifier> := [a-zA-Z]+
 *  8. <vector> := { <constant>, <constant>, <constant> }
 * 
 * Course: CPE2600-121
 * Assignment: Lab Wk 5
 * @date 2023-10-01
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <float.h>

#include "ast.h"
#include "vec.h"
#include "vecvec.h"

/**
 * @brief Returns the next valid token in the input buffer 
 * at positon position
 * 
 * @param input input string
 * @param position current position in the string
 * @return token 
 */
token find_next_token(char* input, int* position) {

    char cur = input[(*position)];
    while(isspace(cur)) {
        cur = input[++(*position)];
    }

    token tok;
    tok.name = NULL;

    switch(cur) {
        case '+':
            tok.name = "+";
            tok.type = TOKEN_PLUS;
            (*position)++;
            break;
        case '-':
            tok.name = "-";
            tok.type = TOKEN_MINUS;
            (*position)++;
            break;
        case '*':
            tok.name = "*";
            tok.type = TOKEN_STAR;
            (*position)++;
            break;
        case '/':
            tok.name = "/";
            tok.type = TOKEN_SLASH;
            (*position)++;
            break;
        case '.':
            tok.name = ".";
            tok.type = TOKEN_DOT;
            (*position)++;
            break;
        case '/':
            tok.name = "X";
            tok.type = TOKEN_CROSS;
            (*position)++;
            break;
        case '=':
            tok.name = "=";
            tok.type = TOKEN_EQUALS;
            (*position)++;
            break;
        case ',':
            tok.name = ",";
            tok.type = TOKEN_COMMA;
            (*position)++;
            break;
        case '\0':
            tok.name = "EOF";
            tok.type = TOKEN_END;
            (*position)++;
            break;
        case '(':
            tok.name = "(";
            tok.type = TOKEN_LPAREN;
            (*position)++;
            break;
        case ')':
            tok.name = ")";
            tok.type = TOKEN_RPAREN;
            (*position)++;
            break;
        case '{':
            tok.name = "{";
            tok.type = TOKEN_LBRACKET;
            (*position)++;
            break;
        case '}':
            tok.name = "}";
            tok.type = TOKEN_RBRACKET;
            (*position)++;
            break;
        default: 
            if(isalpha(cur)) {
                tok.type = TOKEN_IDENTIFIER;
                
                int size = 1;

                while(isalnum(input[*position + size])) {
                    size++;
                }

                tok.name = malloc(size + 1);
                memcpy(tok.name, (input + (*position)), size);
                tok.name[size] = '\0';
                (*position) += size;
            } else if (isdigit(cur) || cur == '.') {
                // Parse numbers (integer or floating-point)
                tok.type = TOKEN_CONST;

                int size = 1;

                while (isdigit(input[*position + size]) || input[*position + size] == '.') {
                    size++;
                }

                tok.name = malloc(size + 1);
                memcpy(tok.name, (input + (*position)), size);
                tok.name[size] = '\0';
                (*position) += size;

            } else {
                // not sure
            }
    }
    return tok;
}

/**
 * @brief Lexes the input string and returns a list of valid tokens
 * 
 * @param input Input string
 * @return token* 
 */
token* lex(char* input) {
    int position = 0;
    int capacity = 100;
    int size = 0;
    token* tokens = malloc(capacity * sizeof(token));
    while(1) {
        token tok = find_next_token(input, &position);

        tokens[size++] = tok;

        if(tok.type == TOKEN_END) {
            break;
        }
    }
    return tokens;
}


/**
 * @brief Create a node struct
 * 
 * @param type Type of node
 * @param value Node value
 * @param left Left Child
 * @param right Right Child
 * @return node* 
 */
node* create_node(node_type type, char* value, node* left, node* right) {
    node* n = (node*) malloc(sizeof(node));
    n->type = type;
    n->value = value;
    n->left = left;
    n->right = right;
    n->is_root = 0;
    return n;
}


node* parse_statement(token *tokens, int *position);
node* parse_expression(token *tokens, int *position);
node* parse_term(token *tokens, int *position);
node* parse_factor(token *tokens, int *position);
node* parse_identifier(token *tokens, int *position);
node* parse_constant(token *tokens, int *position);
node* parse_value(token *tokens, int *position);
node* parse_assignment(token* tokens, int* position);

// static value ans = sentinel();
node* parse_input(char* input) {
    token* tokens = lex(input);
    int position = 0;
    node* root = parse_statement(tokens, &position);
    root->is_root = 1;
    return root;
}

// value ans() {
//     return ans;
// }

node* parse_statement(token *tokens, int* position) {
    if(tokens[*position].type == TOKEN_IDENTIFIER && tokens[*position + 1].type == TOKEN_EQUALS) {
        return parse_assignment(tokens, position);
    } else {
        return parse_expression(tokens, position);
    }
}

/**
 * @brief 
 *        =
 *   <id>    <exp>
 * @param tokens 
 * @param position 
 * @return node* 
 */
node* parse_assignment(token* tokens, int* position) {
    node* identifier = parse_identifier(tokens, position);
    (*position)++;  // consume =
    // if(tokens[*position].type == TOKEN_CONST) {
    //     return create_node(NODE_ASSIGNMENT, "=", identifier, parse_value(tokens, position));
    // } else {
        return create_node(NODE_ASSIGNMENT, "=", identifier, parse_expression(tokens, position));
    // }
}

node* parse_expression(token* tokens, int* position) {
    node* term = parse_term(tokens, position);
    while(tokens[*position].type == TOKEN_PLUS || tokens[*position].type == TOKEN_MINUS) {
        char* operator = tokens[(*position)].name;
        (*position)++;
        node* right = parse_term(tokens, position);
        term = create_node(NODE_OPERATION, operator, term, right);
    }
    return term;
}

node* parse_term(token* tokens, int* position) {
    node* factor = parse_factor(tokens, position);

    while(tokens[*position].type == TOKEN_STAR || tokens[*position].type == TOKEN_SLASH) {
        char* operator = tokens[(*position)].name;
        (*position)++;
        node* right = parse_factor(tokens, position);
        factor = create_node(NODE_OPERATION, operator, factor, right);
    }
    return factor;
}

/**
 * @brief 
 * <factor> -> <id> | V | (<exp>)
 * @param tokens 
 * @param position 
 * @return node* 
 */
node* parse_factor(token* tokens, int* position) {
    if(tokens[*position].type == TOKEN_LPAREN) {
        (*position)++;  // consume ()
        node* expression = parse_expression(tokens, position);
        (*position)++;  // consume ()
        return expression;
    } else if(tokens[*position].type == TOKEN_IDENTIFIER) { 
        return parse_identifier(tokens, position);
    // } else if(tokens[*position].type == TOKEN_LBRACKET) { 
    } else if(tokens[*position].type == TOKEN_CONST) { 
        return parse_value(tokens, position);
    } else {
        printf("error at position %d in parse_factor\n", *position);
        // error condition;
        return NULL;
    }


}

/**
 * @brief 
 * Constants are terminal values
 * @param tokens 
 * @param position 
 * @return node* 
 */
node* parse_constant(token* tokens, int* position) {
    char* value = tokens[(*position)].name;
    (*position )++;
    return create_node(NODE_CONSTANT, value, NULL, NULL);
}


/**
 * @brief 
 *  V -> { <const>, <const>, <const> }
 * @param tokens 
 * @param position 
 * @return node* 
 */
node* parse_value(token* tokens, int* position) {
    if(tokens[*position].type == TOKEN_CONST) {
        node* i = parse_constant(tokens, position);
        if(tokens[*position].type == TOKEN_COMMA) {
            (*position)++;
        }

        node* j;
        node* k;

        token next = tokens[*position];
        if(next.type != TOKEN_END && next.type == TOKEN_CONST) {
            j = parse_constant(tokens, position);
            if(tokens[*position].type == TOKEN_COMMA) {
                (*position)++;
            }
            if(tokens[*position].type != TOKEN_END) {
                k = parse_constant(tokens, position);
                if(tokens[*position].type == TOKEN_COMMA) {
                    (*position)++;
                }
            } else {
                k = create_node(NODE_CONSTANT, "0", NULL, NULL);
            }
//        } else if(next.type != TOKEN_END && next.type != TOKEN_CONST) {
        } else {
            printf("parse_value found constant\n");
            printf("type: %d\n", i->type);
            return i;
        }

        return create_node(NODE_VECTOR, NULL, i, create_node(NODE_VECTOR, NULL, j, k));
    } else {
        return NULL;
    }
}

node* parse_identifier(token* tokens, int* position) {
    char* name = tokens[(*position)].name;
    (*position)++;
    return create_node(NODE_IDENTIFIER, name, NULL, NULL);
}


void free_ast(node* n) {
    if (n == NULL) {
        return;
    }

    free_ast(n->left);
    free_ast(n->right);
    free(n);
}

void print_ast_recursive(node* node, int depth) {
    if (node == NULL) {
        return;
    }

    // Print indentation based on depth
    for (int i = 0; i < depth; ++i) {
        printf("  ");
    }

    // Print node information
    printf("Type: %d, Value: %s\n", node->type, node->value);

    // Recursively print left and right children
    print_ast_recursive(node->left, depth + 1);
    print_ast_recursive(node->right, depth + 1);
}

void print_ast(node* root) {
    printf("Abstract Syntax Tree:\n");
    print_ast_recursive(root, 0);
}

char* value_to_string(value v) {
    static char buffer[200];
    if(!is_sentinel(v)) {
        if(v.type == VAL_VECTOR) {
            snprintf(buffer, 200, "%s\n", vector_to_string(v.vec));
        } else {
            snprintf(buffer, 200, "%.2f\n", v.scalar);
        }
    }
    return buffer;

}

value make_value_from_vector(vector v) {
    value r;
    r.type = VAL_VECTOR;
    r.vec = v;
    return r;
}

value make_value_from_scalar(float f) {
    value r;
    r.type = VAL_SCALAR;
    r.scalar = f;
    return r;
}

value sentinel() {
    value r ;
    r.type = VAL_SENTINEL;
    r.scalar;
    return r;
}

int is_sentinel(value s) {
    return s.type == VAL_SENTINEL;
}

value handle_asssignemnt(node* n) {
    if(n->left == NULL || n->left->type != NODE_IDENTIFIER || n->right == NULL) {
        printf("Error: empty node\n");
        return sentinel();
    }

    value result = evaluate_ast(n->right);
    if(!is_sentinel(result)) {
        if(result.type == VAL_VECTOR) {
            insert_vector(result.vec, n->left->value);
            return result;
        } else {
            printf("Warning: Cannot assign scalar to variable\n");
            printf("Assigning scalar as field i\n");
            vector v = {result.scalar, 0, 0};
            insert_vector(v, n->left->value);
            value r;
            r.type = VAL_VECTOR;
            r.vec = v;
            return r;
        }
    } else {
        return sentinel();
    }

}

void print_help() {
    printf("tritone: very bad vector calculator\n" 
           "- store a vector: a = 1, 2, 3\n"
           "- scalar operations: 1+2, 6-9, 5*3, 9/1,\n"
           "- vector operations: a + b, a + (1, 2, 3 * c)\n" 
           "\t-supports addition, subtraction, scalar multiplication," 
           " scalar division, cross product, dot product.\n"
           );
}

value handle_identifier(node* n) {
    if(!strcmp(n->value, "quit")) {
        exit(0);
    } else if(!strcmp(n->value, "free")) {
        int cleared = clear_vectors();
        printf("Freed %d vectors\n", cleared);
        return sentinel();
    } else if(!strcmp(n->value, "list")) {
        list_vectors();
        return sentinel();
    } else if(!strcmp(n->value, "help")) {
        print_help();
        return sentinel();
    } else if(!strcmp(n->value, "clear")) {
        printf("\033[2J"); // clear screen
        printf("\033[H"); // go home

        return sentinel();
    } else {
        vec_cell* v = get_vector(n->value);
        if(v != NULL) {
            return make_value_from_vector(v->vec);
        } else {
            printf("Error: no vector found named %s\n", n->value);
            return sentinel();
        }
    }
}



value evaluate_ast(node* n) {
    if(n == NULL) {
        // TODO: Handle this case
        printf("Error: empty node");
        return sentinel();
    } 

    switch(n->type) {
        case(NODE_OPERATION):
            if(!strcmp(n->value, "+")) {

                value left = evaluate_ast(n->left);
                value right = evaluate_ast(n->right);

                if(left.type == VAL_VECTOR && right.type == VAL_VECTOR) {
                    vector sum = vec_add(left.vec, right.vec);
                    return make_value_from_vector(sum);
                } else if(left.type == VAL_SCALAR && right.type == VAL_SCALAR) { 
                    float sum = left.scalar + right.scalar;
                    value v = make_value_from_scalar(sum);
                    return v;
                } else {
                    printf("Error: addition not implemented for scalar + vector\n");
                    return sentinel();
                }
            } else if(!strcmp(n->value, "-")) { 
                value left = evaluate_ast(n->left);
                value right = evaluate_ast(n->right);

                if(left.type == VAL_VECTOR && right.type == VAL_VECTOR) {
                    vector sum = vec_sub(left.vec, right.vec);
                    return make_value_from_vector(sum);
                } else if(left.type == VAL_SCALAR && right.type == VAL_SCALAR) { 
                    float sum = left.scalar - right.scalar;
                    return make_value_from_scalar(sum);
                } else {
                    printf("Error: subtraction not implemented for scalar + vector\n");
                    return sentinel();
                }
            } else if(!strcmp(n->value, "*")) { 

                value left = evaluate_ast(n->left);
                value right = evaluate_ast(n->right);

                if(left.type == VAL_VECTOR && right.type == VAL_VECTOR) {
                    vector sum = vec_mul(left.vec, right.vec);
                    return make_value_from_vector(sum);
                } else if(left.type == VAL_SCALAR && right.type == VAL_SCALAR) { 
                    float sum = left.scalar * right.scalar;
                    return make_value_from_scalar(sum);
                } else if(left.type == VAL_VECTOR && right.type == VAL_SCALAR) {
                    float i = left.vec.i * right.scalar;
                    float j = left.vec.j * right.scalar;
                    float k = left.vec.k * right.scalar;
                    vector product = {i, j, k};
                    return make_value_from_vector(product);
                } else {
                    float i = right.vec.i * left.scalar;
                    float j = right.vec.j * left.scalar;
                    float k = right.vec.k * left.scalar;
                    vector product = {i, j, k};
                    return make_value_from_vector(product);
                }
            } else {
                // err
            }
            break;
        case(NODE_IDENTIFIER):
            return handle_identifier(n);
            break;
        case(NODE_ASSIGNMENT):
            return handle_asssignemnt(n);
            break;
        case(NODE_VECTOR):
            float i = atof(n->left->value);
            float j = atof(n->right->left->value);
            float k = atof(n->right->right->value);
            vector v = {i, j, k};
            return make_value_from_vector(v);
        case(NODE_CONSTANT):
            return make_value_from_scalar(atof(n->value));
        default:
    }
}
