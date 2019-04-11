%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "list.h"
%}

/* regexp definitions */
delim [ \t\n]
ws {delim}+
digit [0-9]
letter [A-Za-z]
id {letter}({letter}|{digit})*
integer (0|([1-9]{digit}*))
number {integer}(\.{digit}+)?((E|e)[+\-]?{digit}+)?
operator [+\-\*/]

%%

{number} { return NUMBER; }
{id} { return IDENTIFIER; }
{operator} { return OPERATOR; }
{ws} { return WHITESPACE; }
. { return OTHER; }

%%

/* 없으면 "ld: symbol(s) not found for architecture x86_64" 컴파일 에러 발생 */
int yywrap() {}

int main()
{
        int token;
        struct token_struct *temp_tok;
        struct list_node in_list, *post_list;

        list_init(&in_list);

        /* 숫자, 식별자, 연산자, 공백은 토큰 리스트에 삽입된다. */
        while (token = yylex()) {
                switch (token) {
                case NUMBER:
                case IDENTIFIER:
                case OPERATOR:
                case WHITESPACE:
                        temp_tok = malloc(sizeof(struct token_struct));
                        strcpy(temp_tok->str, yytext);
                        temp_tok->tok_type = token;
                        list_insert(&temp_tok->tok_list, in_list.prev);
                        break;
                default:
                        printf("Error: %s not recognized\n", yytext);
                }
        }

        /* 토큰 리스트에서 음수 부호로 처리될 수 있는 "-"를 변환 */
        chg_minoper_neg(&in_list);

        /* 토큰 리스트에 있는 모든 공백을 제거 */
        rmv_whtspc(&in_list);

        /* infix 토큰 리스트를 postfix 토큰 리스트로 변환 */
        post_list = infix_to_postfix(&in_list);

        for_each_list_node(*post_list) {
                temp_tok = container_of(node, struct token_struct, tok_list);
                printf("%s ", temp_tok->str);
        }

        return 0;
}