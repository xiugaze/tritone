#ifndef AST_H
#define AST_H
    #include "vec.h"

    typedef enum {
        TOKEN_IDENTIFIER,
        TOKEN_EQUALS,
        TOKEN_COMMA,
        TOKEN_PLUS,
        TOKEN_MINUS,
        TOKEN_STAR,
        TOKEN_SLASH,
        TOKEN_END,
        TOKEN_LPAREN,
        TOKEN_RPAREN,
        TOKEN_LBRACKET,
        TOKEN_RBRACKET,
        TOKEN_DOT,
        TOKEN_CROSS,
        TOKEN_CONST
    } token_type;

    typedef struct {
        token_type type;
        char* name;
    } token;

    typedef enum {
        NODE_ASSIGNMENT,
        NODE_OPERATION,
        NODE_IDENTIFIER,
        NODE_VECTOR,
        NODE_CONSTANT,
    } node_type;

    typedef struct node node;
    struct node {
        char* value;
        node_type type;
        int is_root;
        node* left;
        node* right;
    };


    typedef enum {
        VAL_VECTOR,
        VAL_SCALAR,
        VAL_SENTINEL,
    } value_type;

    typedef struct {
        value_type type;
        union {
            float scalar;
            vector vec;
        };
    } value;

    node* parse_input(char* input);
    void print_ast(node* root);
    value evaluate_ast(node* n);
    char* value_to_string(value v);

#endif 