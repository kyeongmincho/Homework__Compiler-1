/*
 * 토큰과 관련된 구조체와
 * 음수 처리 함수, infix_to_postfix 함수,
 * 공백 제거 함수, 그리고 간단한 타입 체크 함수를 제공합니다.
 */

#ifndef TOKEN_H
#define TOKEN_H

#include "list.h"

#define NR_TOKEN_STR 105

enum token_struct_type {
        NUMBER = 256,
        IDENTIFIER,
        OPERATOR,
        WHITESPACE,
        OTHER
};

struct token_struct {
        /* 토큰이 나타내는 문자열 */
        char str[NR_TOKEN_STR];

        /* 토큰의 타입 */
        int tok_type;

        /* 토큰 리스트의 노드 */
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

static inline int is_number(struct token_struct *tok)
{
        return (tok->tok_type == NUMBER);
}

static inline int is_whtspc(struct token_struct *tok)
{
        return (tok->tok_type == WHITESPACE);
}

struct list_node *infix_to_postfix(struct list_node *);
void chg_minoper_neg(struct list_node *);
void rmv_whtspc(struct list_node *);

#endif // TOKEN_H
