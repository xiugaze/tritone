/**
 * @file ast.c
 * @author Caleb Andreano (andreanoc@msoe.edu)
 * @class CPE2600-121
 * @brief Helper routines for parsing an input string, constructing
 * an abstract syntax tree according to context free grammar G,
 * and evaluating the tree to a final result. Assignments are expressions,
 * not statements, and evaluuate to the left hand side.
 *
 * 
 * Let G := 
 *  1. <statement> := <statement><statement> 
 *  2. <statement> := <assignment> | <expression>
 *  3. <assignment> := <identifier> = <expression> 
 *  4. <expression> := <term> | <term> { + | - } <expression> 
 *  5. <term> := <factor> | <factor> { * | / | .| X } <term>
 *  6. <factor> := <identifier> | <vector> | <constant> |(<expression>)
 *  7. <identifier> := [a-zA-Z]+
 *  8. <value> := { <constant> | <constant>, <constant>, <constant> }
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
        case 'X':
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

                while (isdigit(input[*position + size]) 
                    || input[*position + size] == '.') {
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

/**
 * @brief Lexes and parses an input string according to G
 * 
 * @param input 
 * @return node* 
 */
node* parse_input(char* input) {
    token* tokens = lex(input);
    int position = 0;
    return parse_statement(tokens, &position);
}

/**
 * @brief Parses a statement and returns its root node
 *  <statement> := <assignment> | <expression>
 * 
 * @param tokens 
 * @param position 
 * @return node* 
 */
node* parse_statement(token *tokens, int* position) {
    if(tokens[*position].type == TOKEN_IDENTIFIER 
        && tokens[*position + 1].type == TOKEN_EQUALS) {
        return parse_assignment(tokens, position);
    } else if(tokens[*position].type == TOKEN_EQUALS) {
        printf("Error: assignment with no identifier\n");
        return NULL;
    } else {
        return parse_expression(tokens, position);
    }
}

/**
 * @brief 
 * Parses an assignment and returns its root node
 * <assignment> := <identifier> = <expression> 
 * @param tokens 
 * @param position 
 * @return node* 
 */
node* parse_assignment(token* tokens, int* position) {
    node* identifier = parse_identifier(tokens, position);
    (*position)++;  
    return create_node(
        NODE_ASSIGNMENT, 
        "=", 
        identifier, 
        parse_expression(tokens, position)
    );
}

node* parse_expression(token* tokens, int* position) {
    node* term = parse_term(tokens, position);
    while(tokens[*position].type == TOKEN_PLUS 
       || tokens[*position].type == TOKEN_MINUS) {
        char* operator = tokens[(*position)].name;
        (*position)++;
        node* right = parse_term(tokens, position);
        term = create_node(NODE_OPERATION, operator, term, right);
    }
    return term;
}

/**
 * @brief Parses a term and returns it's root node
 * <term> := <factor> | <factor> { * | / | .| X } <term>
 * @param tokens 
 * @param position 
 * @return node* 
 */
node* parse_term(token* tokens, int* position) {
    node* factor = parse_factor(tokens, position);

    while(
            tokens[*position].type == TOKEN_STAR || 
            tokens[*position].type == TOKEN_SLASH || 
            tokens[*position].type == TOKEN_CROSS || 
            tokens[*position].type == TOKEN_DOT
        ) {
        char* operator = tokens[(*position)].name;
        (*position)++;
        node* right = parse_factor(tokens, position);
        factor = create_node(NODE_OPERATION, operator, factor, right);
    }
    return factor;
}

/**
 * @brief 
 * Parses a factor and returns its root node
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
    } else if(tokens[*position].type == TOKEN_CONST) { 
        return parse_value(tokens, position);
    } else {
        printf("Error at position %d near token '%s'\n",
            *position, tokens[*position-1].name);
        return NULL;
    }
}

/**
 * @brief 
 * Parses a constant value
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
 *  Parses a value and returns its root node
 *  V -> { <const> | <const>, <const>, <const> }
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
        // If there's two constants in a row
        if(next.type != TOKEN_END && next.type == TOKEN_CONST) {
            j = parse_constant(tokens, position);

            if(tokens[*position].type == TOKEN_COMMA) {
                (*position)++;
            }

            // If there's three constants
            if(tokens[*position].type != TOKEN_END) {
                k = parse_constant(tokens, position);
                if(tokens[*position].type == TOKEN_COMMA) {
                    (*position)++;
                }

                // If there's four right night to eachother
                if(tokens[*position].type == TOKEN_CONST) {
                    printf("Warning: Only 3D vectors are supported. Using"
                           " first three tokens.\n");
                };

            } else {
                k = create_node(NODE_CONSTANT, "0", NULL, NULL);
            }
            return create_node(NODE_VECTOR, NULL, i, create_node(NODE_VECTOR, NULL, j, k));
        } else {
            // there's only one constant
            return i;
        }
    } else {
        return NULL;
    }
}




/**
 * @brief Parses an identifier and returns it's node
 * 
 * <identifier> := [a-zA-Z]+
 * @param tokens 
 * @param position 
 * @return node* 
 */
node* parse_identifier(token* tokens, int* position) {
    char* name = tokens[(*position)].name;
    (*position)++;
    return create_node(NODE_IDENTIFIER, name, NULL, NULL);
}


/**
 * @brief Recursively frees a tree given its root node
 * 
 * @param n root node
 */
void free_ast(node* n) {
    if (n == NULL) {
        return;
    }

    free_ast(n->left);
    free_ast(n->right);
    free(n);
}

/**
 * @brief Recursively prints an AST node given its depth
 * 
 * @param node 
 * @param depth 
 */
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

/**
 * @brief Recursively prints a tree given it's root node
 * 
 * @param root 
 */
void print_ast(node* root) {
    printf("Abstract Syntax Tree:\n");
    print_ast_recursive(root, 0);
}

/**
 * @brief Converts a vector to a value struct
 * 
 * @param v 
 * @return value 
 */
value make_value_from_vector(vector v) {
    value r;
    r.type = VAL_VECTOR;
    r.vec = v;
    return r;
}

/**
 * @brief Converts a scalar type to a struct
 * 
 * @param f 
 * @return value 
 */
value make_value_from_scalar(float f) {
    value r;
    r.type = VAL_SCALAR;
    r.scalar = f;
    return r;
}

/**
 * @brief Returns the sentinel value struct
 * 
 * @return value 
 */
value sentinel() {
    value r ;
    r.type = VAL_SENTINEL;
    return r;
}

/**
 * @brief returns true if a value is the sentinel value
 * 
 * @param s 
 * @return int 
 */
int is_sentinel(value s) {
    return s.type == VAL_SENTINEL;
}

/**
 * @brief Converts a value to a string and returns it
 * 
 * @param v 
 * @return char* 
 */
char* value_to_string(value v) {
    static char buffer[200];
    if(!is_sentinel(v)) {
        if(v.type == VAL_VECTOR) {
            snprintf(buffer, 200, "%s\n", vector_to_string(v.vec));
        } else {
            snprintf(buffer, 200, "%.2f\n", v.scalar);
        }
    } else {
        snprintf(buffer, 200, "%s", "");
    }
    return buffer;

}

/**
 * @brief Evaluates an assignment node and returns it's value
 * 
 * @param n 
 * @return value 
 */
value handle_asssignemnt(node* n) {
    if(n->left == NULL || n->left->type != NODE_IDENTIFIER || n->right == NULL) {
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
           " this is a change for git to pick up\n"
           );
}

/**
 * @brief Handles identifiers nodes and processes relevant commands.
 * If the identifier is not a command, returns the value, otherwise sentinel
 * 
 * @param n 
 * @return value 
 */
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

/**
 * @brief Handles vector and scalar operation nodes and returns their value
 * 
 * @param n 
 * @return value 
 */
value handle_operation(node*n) {
    value left = evaluate_ast(n->left);
    value right = evaluate_ast(n->right);

    // Addition operations
    if(!strcmp(n->value, "+")) {

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
    // Subtraction Operations
    } else if(!strcmp(n->value, "-")) { 

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
    // Multiplicaton functions
    } else if(!strcmp(n->value, "*")) { 

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
    // Division operations
    } else if(!strcmp(n->value, "/")) { 

        if(left.type == VAL_SCALAR && right.type == VAL_SCALAR) { 
            return make_value_from_scalar(left.scalar/right.scalar);
        } else {
            printf("Error: invalid arguments to scalar division\n");
            return sentinel();
        }
    // Dot produt
    } else if(!strcmp(n->value, ".")) { 

        if(left.type == VAL_VECTOR && right.type == VAL_VECTOR) {
            float sum = vec_dot(left.vec, right.vec);
            return make_value_from_scalar(sum);
        } else { 
            printf("Error: invalid arguments to dot product\n");
            return sentinel();
        }
    // Cross product
    } else if(!strcmp(n->value, "X")) { 

        if(left.type == VAL_VECTOR && right.type == VAL_VECTOR) {
            vector cross = vec_cross(left.vec, right.vec);
            return make_value_from_vector(cross);
        } else { 
            printf("Error: invalid arguments to cross product\n");
            return sentinel();
        }
    } else {
        return sentinel();
    }
}

/**
 * @brief Handles constructing vectors from vector root nodes:
 * Vectors in memory are represented as: 
 *          
 *              NODE_VECTOR: null
 *                 /         \
 *                /           \
 *      NODE_CONSTANT: i       NODE_VECTOR: null
 *                              /           \ 
 *                  NODE_CONSTANT: j     NODE_CONSTANT: k
 * 
 * @param n 
 * @return value 
 */
value handle_vector(node* n) {
    float i = atof(n->left->value);
    float j = atof(n->right->left->value);
    float k = atof(n->right->right->value);
    vector v = {i, j, k};
    return make_value_from_vector(v);
}

/**
 * @brief Evaluates an AST branch given the root node using
 * recursive descent parsing (essentially pre-order traversal).
 * Returns its value. 
 * 
 * @param n 
 * @return value 
 */
value evaluate_ast(node* n) {
    if(n == NULL) {
        return sentinel();
    } 

    switch(n->type) {
        case(NODE_OPERATION):
            return handle_operation(n);
            break;
        case(NODE_IDENTIFIER):
            return handle_identifier(n);
            break;
        case(NODE_ASSIGNMENT):
            return handle_asssignemnt(n);
            break;
        case(NODE_VECTOR):
            return handle_vector(n);
            break;
        case(NODE_CONSTANT):
            return make_value_from_scalar(atof(n->value));
        default:
            return sentinel();
    }
}
