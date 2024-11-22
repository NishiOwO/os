/* $Id$ */

%{
#include "y.tab.h"

int yylex();
int yyerror(const char*);
%}

%union {
	struct {
		char* value;
		int type;
	} scalar;
}

%start statement_list
%token NEWLINE STRING IDENT SPACES OPTION

%%

statement_list	:	statement
		|	statement_list statement;

statement	:	option NEWLINE
		|	NEWLINE;
	
option		:	OPTION SPACES STRING;

%%
