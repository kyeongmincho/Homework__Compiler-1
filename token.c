#include "token.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct list_node *infix_to_postfix(struct list_node *in_list)
{
        struct token_struct *right, *oper, *left;
        struct list_node *left_list;

        if (list_empty(in_list)) {
                left_list = in_list;
                goto out;
        }

        right = container_of(in_list->prev, struct token_struct, tok_list);
//        printf("\n%s\n", right->str);
//        printf("%d\n", right->tok_type);

        if (!is_value(right)) {
                printf("Not an infix expression due to:\n\t-> %s is not a "
                       "value.", right->str);
                goto err;
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
                goto err;
        }

        if (!strcmp(oper->str, "-") && is_number(right)) {
                if (in_list->prev->prev->prev == in_list) {
                        chg_minoper_neg(oper, right);
                        left_list = in_list;
                        goto out;
                }

                left = container_of(in_list->prev->prev->prev,
                                    struct token_struct, tok_list);

                if (is_operator(left)) {
                        chg_minoper_neg(oper, right);
                        oper = left;
                }
        }

        if (in_list->prev->prev->prev == in_list) {
                printf("Incomplete infix expression.");
                goto err;
        }

        list_delete(&right->tok_list);
        list_delete(&oper->tok_list);
        left_list = infix_to_postfix(in_list);
        list_insert(&right->tok_list, left_list->prev);
        list_insert(&oper->tok_list, left_list->prev);

out:
        return left_list;
err:
        exit(1);
}

void chg_minoper_neg(struct token_struct *minus, struct token_struct *right_num)
{
        strcat(minus->str, right_num->str);
        strcpy(right_num->str, minus->str);
        list_delete(&minus->tok_list);
        free(minus);
}
