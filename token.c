#include "token.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct list_node *infix_to_postfix(struct list_node *in_list)
{
        struct token_struct *right, *oper;
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

/* chg_minoper_neg:
 * 빼기(-) 연산자를 음수 부호로 바꿔준다.
 *
 * 아이디어:
 * 1. 토큰 리스트를 순회하면서 빼기 연산자인지 확인한다.
 * 2. 어떤 토큰이 빼기 연산자일 경우
 *      - 왼쪽 토큰이 없으면서 오른쪽 토큰이 숫자라면 음수 부호다.
 *      - 왼쪽 토큰이 연산자이면서 오른쪽 토큰이 숫자라면 음수 부호다.
 *      (s.t. 왼쪽 토큰은 공백을 제외한 최초의 왼쪽 인접 토큰이다.)
 */
void chg_minoper_neg(struct list_node *t_list)
{
        struct token_struct *tok, *prev, *next;
        struct list_node *prev_node;

        for_each_list_node(*t_list) {
                tok = container_of(node, struct token_struct, tok_list);

                if (!(is_operator(tok) && !strcmp(tok->str, "-")))
                        continue;

                /* 화이트스페이스들을 제외한 최초의 왼쪽 인접 토큰을 구함
                 * 왼쪽 인접 토큰이 없거나 공백이 아닐 경우 진행.
                 * 그렇지 않으면 음수부호가 아니다. */
                prev_node = node->prev;
                while (prev_node != t_list) {
                        prev = container_of(prev_node,
                                            struct token_struct,
                                            tok_list);

                        if (is_operator(prev))
                                break;
                        else if (!is_whtspc(prev))
                                goto loop;

                        prev_node = prev_node->prev;
                }

                /* Now here, there is no prev, or prev is oper
                 * (except whitespace) */

                if (node->next == t_list)
                        continue;

                next = container_of(node->next, struct token_struct, tok_list);

                if (!is_number(next))
                        continue;

                /* The next token is number. So now, "(-)(number)"
                 * Just merge these two tokens. */
                strcat(tok->str, next->str);
                strcpy(next->str, tok->str);
                list_delete(&tok->tok_list);
                free(tok);
                node = &next->tok_list;
loop:           ;
        }
}

/* rmv_whtspc:
 * 토큰 리스트의 공백을 모두 제거한다.
 *
 * 리스트를 순회하면서 토큰은 리스트에서 제거한다.
 */
void rmv_whtspc(struct list_node *t_list)
{
        struct token_struct *tok;

        for_each_list_node(*t_list) {
                tok = container_of(node, struct token_struct, tok_list);

                if (!is_whtspc(tok))
                        continue;

                node = node->prev;
                list_delete(&tok->tok_list);
                free(tok);
        }
}