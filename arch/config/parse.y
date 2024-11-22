/* $Id$ */

%{
#include "y.tab.h"

int yylex();
int yyerror(const char*);
%}

%start statement_list
%token NEWLINE

%%

statement_list	:	statement
		|	statement_list statement;

statement	:	option NEWLINE;

%%
