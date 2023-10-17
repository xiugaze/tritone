/**
 * @file ast.c
 * @author Caleb Andreano (andreanoc@msoe.edu)
 * @class CPE2600-121
 * @brief Helper routines for parsing an input string, constructing
 * an abstract syntax tree according to context free grammar G,
 * and evaluating the tree to a final result. Assignments are expressions,
 * not statements, and evaluate to the left hand side.
 * 
 * Course: CPE2600-121
 * Assignment: Lab Wk 5
 * @date 2023-10-01
 */

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
        NODE_EXECUTE,
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
    void free_ast(node* root);
    value evaluate_ast(node* n);
    char* value_to_string(value v);
    void print_help();

#endif 