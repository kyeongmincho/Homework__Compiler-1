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

int yywrap() {}
int main()
{
        int token;
        struct token_struct *temp_tok;
        struct list_node in_list, *post_list;

        list_init(&in_list);

        while (token = yylex()) {
                switch (token) {
                case NUMBER:
                case IDENTIFIER:
                case OPERATOR:
                        temp_tok = malloc(sizeof(struct token_struct));
                        strcpy(temp_tok->str, yytext);
                        temp_tok->tok_type = token;
                        list_insert(&temp_tok->tok_list, in_list.prev);
                        break;
                case WHITESPACE:
                        break;
                default:
                        printf("Error: %s not recognized\n", yytext);
                }
        }

        post_list = infix_to_postfix(&in_list);

        for_each_list_node(*post_list) {
                temp_tok = container_of(node, struct token_struct, tok_list);
                printf("%s ", temp_tok->str);
        }

        return 0;
}