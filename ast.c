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
    return n;
}


node* parse_statement(token *tokens, int *position);
node* parse_expression(token *tokens, int *position);
node* parse_term(token *tokens, int *position);
node* parse_factor(token *tokens, int *position);
node* parse_identifier(token *tokens, int *position);
node* parse_constant(token *tokens, int *position);
node* parse_vector(token *tokens, int *position);
node* parse_assignment(token* tokens, int* position);

node* parse_input(char* input) {
    token* tokens = lex(input);
    int position = 0;
    return parse_statement(tokens, &position);
}

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
    if(tokens[*position].type == TOKEN_LBRACKET) {
        return create_node(NODE_ASSIGNMENT, "=", identifier, parse_vector(tokens, position));
    } else {
        return create_node(NODE_ASSIGNMENT, "=", identifier, parse_expression(tokens, position));
    }
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
        return parse_vector(tokens, position);
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
node* parse_vector(token* tokens, int* position) {
    // if(tokens[*position].type == TOKEN_CONST) {
    //     // consume {
    //     (*position)++;
    //     node* i = parse_constant(tokens, position);
    //     if(tokens[*position].type == TOKEN_COMMA) {
    //         (*position)++;
    //     }
    //     node* j = parse_constant(tokens, position);
    //     if(tokens[*position].type == TOKEN_COMMA) {
    //         (*position)++;
    //     }
    //     node* k = parse_constant(tokens, position);
    //     if(tokens[*position].type == TOKEN_COMMA) {
    //         (*position)++;
    //     }
    //     // consume {
    //     (*position)++;
    if(tokens[*position].type == TOKEN_CONST) {
        node* i = parse_constant(tokens, position);
        if(tokens[*position].type == TOKEN_COMMA) {
            (*position)++;
        }

        node* j;
        node* k;

        if(tokens[*position].type != TOKEN_END) {
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
        } else {
            j = create_node(NODE_CONSTANT, "0", NULL, NULL);
            k = create_node(NODE_CONSTANT, "0", NULL, NULL);
        }

        return create_node(NODE_VECTOR, NULL, i, create_node(NODE_VECTOR, NULL, j, k));
    } else {
        // help
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


vector handle_asssignemnt(node* n) {
    if(n->left == NULL || n->left->type != NODE_IDENTIFIER || n->right == NULL) {
        printf("Error: empty node\n");
        return vec_max();
    }
    vector result = evaluate_ast(n->right);
    if(!is_max(result)) {
        insert_vector(result, n->left->value);
    }
    return result;

}

vector handle_identifier(node* n) {
    if(!strcmp(n->value, "quit")) {
        exit(0);
    } else if(!strcmp(n->value, "clear")) {
        int cleared = clear_vectors();
        printf("Cleared %d vectors\n", cleared);
        return vec_max();
    } else if(!strcmp(n->value, "list")) {
        list_vectors();
        return vec_max();
    } else {
        vec_cell* v = get_vector(n->value);
        if(v != NULL) {
            return v->vec;
        } else {
            printf("Error: no vector found named %s\n", n->value);
            vector ret = vec_max();
            return ret;
        }
    }
}
vector evaluate_ast(node* n) {
    if(n == NULL) {
        // TODO: Handle this case
        printf("Error: empty node");
        return vec_max();
    } 

    switch(n->type) {
        case(NODE_OPERATION):
            if(!strcmp(n->value, "+")) {
                return vec_add(evaluate_ast(n->left), evaluate_ast(n->right));
            } else if(!strcmp(n->value, "-")) { 
                return vec_sub(evaluate_ast(n->left), evaluate_ast(n->right));
            } else if(!strcmp(n->value, "*")) { 
                return vec_mul(evaluate_ast(n->left), evaluate_ast(n->right));
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
            return v;
        default:
        

    }
}

// int main() {
//     char input[] = "var1 = (a * b) - c";
//     token* tokens = lex(input);
//     int index = 0;


//     int pos = 0;
//     node* n = parse_statement(tokens, &pos);
//     printf("%s\n", n->right->value);
// }
