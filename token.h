#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include "list.h"

#define NR_TOKEN_STR 105

enum token_struct_type {
        NUMBER = 256,
        IDENTIFIER,
        OPERATOR,
        WHITESPACE
};

struct token_struct {
        char str[NR_TOKEN_STR];
        int tok_type;

        struct list_node tok_list;
};

static inline int is_operator(struct token_struct *tok)
{
        return (tok->tok_type == OPERATOR);
}

static inline int is_value(struct token_struct *tok)
{
        return (tok->tok_type == NUMBER
                || tok->tok_type == IDENTIFIER);
}

struct list_node *infix_to_postfix(struct list_node *in_list)
{
        struct token_struct *right, *oper;
        struct list_node *left_list;

        if (list_empty(in_list)) {
                left_list = in_list;
                goto out;
        }

        right = container_of(in_list->prev, struct token_struct, tok_list);
//        printf("%s\n", left->str);

        if (!is_value(right)) {
                printf("Not an infix expression due to:\n\t-> %s is not a "
                       "value.", right->str);
                exit(1);
        }

        if (in_list->prev->prev == in_list) {
                left_list = in_list;
                goto out;
        }

        oper = container_of(in_list->prev->prev, struct token_struct, tok_list);
//        printf("%s\n", oper->str);

        if (!is_operator(oper)) {
                printf("Not an infix expression because:\n\t-> %s is not "
                       "an operator.", oper->str);
                exit(1);
        }

        if (in_list->prev->prev->prev == in_list) {
                printf("Incomplete infix expression.");
                exit(1);
        }

        list_delete(&right->tok_list);
        list_delete(&oper->tok_list);
        left_list = infix_to_postfix(in_list);
        list_insert(&right->tok_list, left_list->prev);
        list_insert(&oper->tok_list, left_list->prev);

out:
        return left_list;
}


#endif // TOKEN_H
