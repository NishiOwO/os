/* $Id$ */

%{
#define DEFINE_PARSE_H
#include "y.tab.h"
#include "parse.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>

int have_program(const char* str);
int yylex(void);
int yyerror(const char*);
void set_cc(const char* prefix, const char* name);
void add_cflags(const char* str);
void add_ldflags(const char* str);

#define DEFPATH "/bin:/sbin:/usr/bin:/usr/sbin"
%}

%union {
	struct {
		char* value;
	} scalar;
}

%start statement_list
%token NEWLINE STRING IDENT SPACES OPTION TARGET

%%

statement_list	:	statement
		|	statement_list statement;

statement	:	option NEWLINE
		|	target NEWLINE
		|	NEWLINE;
	
option		:	OPTION SPACES STRING {
	char str[512];
	str[0] = 0;
	strcat(str, "-D");
	strcat(str, $<scalar.value>3);
	strcat(str, "=1");
	add_cflags(str);
	printf("option: %s\n", $<scalar.value>3);
};

target		:	TARGET SPACES STRING {
	struct utsname uts;
	uname(&uts);
	printf("target: %s\n", $<scalar.value>3);
	printf("host: %s\n", uts.machine);
	add_cflags("-nostdinc");
	add_ldflags("-nostdlib");
	if(strcmp($<scalar.value>3, uts.machine) == 0){
		set_cc(NULL, "cc");
		set_cc(NULL, "gcc");
		printf("target == host, using cc/gcc\n");
	}else if((strcmp("amd64", uts.machine) == 0 || strcmp("x86_64", uts.machine) == 0) && strcmp("i386", $<scalar.value>3) == 0){
		add_cflags("-m32");
		add_ldflags("-m32");
		set_cc(NULL, "cc");
		set_cc(NULL, "gcc");
		printf("target == 32 bit version of host, using cc/gcc with -m32\n");
	}else{
		set_cc($<scalar.value>3, "cc");
		set_cc($<scalar.value>3, "gcc");
		printf("using cross compiler\n");
	}
};

%%

int have_program(const char* str){
	char* path = getenv("PATH");
	char* buf;
	char* p;
	char* old;
	int st = 0;
	if(path == NULL){
		buf = malloc(strlen(DEFPATH) + 1);
		strcpy(buf, DEFPATH);
	}else{
		buf = malloc(strlen(path) + 1);
		strcpy(buf, path);
	}
	old = buf;
	while((p = strtok(old, ":")) != NULL){
		char* name = malloc(strlen(p) + 1 + strlen(str) + 1);
		strcpy(name, p);
		name[strlen(p)] = '/';
		strcpy(name + strlen(p) + 1, str);
		old = NULL;
		st = access(name, F_OK) == 0;
		free(name);
		if(st) break;
	}
	free(buf);
	return st;
}

void set_cc(const char* prefix, const char* name){
	char str[512];
	str[0] = 0;
	if(prefix != NULL){
		strcat(str, prefix);
		strcat(str, "-none-elf-");
	}
	strcat(str, name);
	if(have_program(str)){
		if(cc != NULL) free(cc);
		cc = malloc(strlen(str) + 1);
		strcpy(cc, str);
	}
}

void add_cflags(const char* str){
	if(cflags == NULL){
		cflags = malloc(strlen(str) + 1);
		strcpy(cflags, str);
	}else{
		char* new = malloc(strlen(cflags) + 1 + strlen(str) + 1);
		strcpy(new, cflags);
		new[strlen(cflags)] = ' ';
		strcpy(new + strlen(cflags) + 1, str);
		free(cflags);
		cflags = new;
	}
}

void add_ldflags(const char* str){
	if(ldflags == NULL){
		ldflags = malloc(strlen(str) + 1);
		strcpy(ldflags, str);
	}else{
		char* new = malloc(strlen(ldflags) + 1 + strlen(str) + 1);
		strcpy(new, ldflags);
		new[strlen(ldflags)] = ' ';
		strcpy(new + strlen(ldflags) + 1, str);
		free(ldflags);
		ldflags = new;
	}
}
