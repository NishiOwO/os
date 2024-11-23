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
void set_cpp(const char* prefix, const char* name);
void set_cc(const char* prefix, const char* name);
void set_as(const char* prefix, const char* name);
void set_ld(const char* prefix, const char* name);
void set_ar(const char* prefix, const char* name);
void add_controller(const char* str);
void add_console(const char* str);
void add_defines(const char* str);
void add_cflags(const char* str);
void add_asflags(const char* str);
void add_ldflags(const char* str);

#define DEFPATH "/bin:/sbin:/usr/bin:/usr/sbin"
%}

%union {
	struct {
		char* value;
	} scalar;
}

%start statement_list
%token NEWLINE STRING IDENT SPACES OPTION TARGET CONTROLLER CONSOLE

%%

statement_list	:	statement
		|	statement_list statement;

statement	:	option NEWLINE
		|	target NEWLINE
		|	controller NEWLINE
		|	console NEWLINE
		|	NEWLINE;
	
option		:	OPTION SPACES STRING {
	add_defines($<scalar.value>3);
	printf("option: %s\n", $<scalar.value>3);
};
	
controller	:	CONTROLLER SPACES STRING {
	add_controller($<scalar.value>3);
	printf("controller: %s\n", $<scalar.value>3);
};
	
console	:	CONSOLE SPACES STRING {
	add_console($<scalar.value>3);
	printf("console: %s\n", $<scalar.value>3);
};

target		:	TARGET SPACES STRING {
	struct utsname uts;
	char str[512];
	str[0] = 0;
	uname(&uts);
	printf("target: %s\n", $<scalar.value>3);
	printf("host: %s\n", uts.machine);
	add_cflags("-nostdinc");
	add_ldflags("-nostdlib");
	strcat(str, "TARGET=\\\"");
	strcat(str, $<scalar.value>3);
	strcat(str, "\\\"");
	add_defines(str);
	if(strcmp($<scalar.value>3, uts.machine) == 0){
		set_cpp(NULL, "cpp");
		set_cpp(NULL, "gcpp");
		set_cc(NULL, "cc");
		set_cc(NULL, "gcc");
		set_as(NULL, "as");
		set_as(NULL, "gas");
		set_ld(NULL, "ld");
		set_ld(NULL, "gld");
		set_ar(NULL, "ar");
		set_ar(NULL, "gar");
		printf("target == host, using cc/gcc\n");
	}else if((strcmp("amd64", uts.machine) == 0 || strcmp("x86_64", uts.machine) == 0) && strcmp("i386", $<scalar.value>3) == 0){
		add_cflags("-m32");
		add_asflags("--32");
		set_cpp(NULL, "cpp");
		set_cpp(NULL, "gcpp");
		set_cc(NULL, "cc");
		set_cc(NULL, "gcc");
		set_as(NULL, "as");
		set_as(NULL, "gas");
		set_ld(NULL, "ld");
		set_ld(NULL, "gld");
		set_ar(NULL, "ar");
		set_ar(NULL, "gar");
		printf("target == 32 bit version of host, using cc/gcc with -m32\n");
	}else{
		set_cpp($<scalar.value>3, "cpp");
		set_cpp($<scalar.value>3, "gcpp");
		set_cc($<scalar.value>3, "cc");
		set_cc($<scalar.value>3, "gcc");
		set_as($<scalar.value>3, "as");
		set_as($<scalar.value>3, "gas");
		set_ld($<scalar.value>3, "ld");
		set_ld($<scalar.value>3, "gld");
		set_ar($<scalar.value>3, "ar");
		set_ar($<scalar.value>3, "gar");
		printf("using cross compiler\n");
	}
	str[0] = 0;
	strcat(str, "-melf_");
	strcat(str, $<scalar.value>3);
	add_ldflags(str);
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

void set_cpp(const char* prefix, const char* name){
	char str[512];
	str[0] = 0;
	if(prefix != NULL){
		strcat(str, prefix);
		strcat(str, "-none-elf-");
	}
	strcat(str, name);
	if(have_program(str)){
		if(cpp != NULL) free(cpp);
		cpp = malloc(strlen(str) + 1);
		strcpy(cpp, str);
	}
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

void set_as(const char* prefix, const char* name){
	char str[512];
	str[0] = 0;
	if(prefix != NULL){
		strcat(str, prefix);
		strcat(str, "-none-elf-");
	}
	strcat(str, name);
	if(have_program(str)){
		if(as != NULL) free(as);
		as = malloc(strlen(str) + 1);
		strcpy(as, str);
	}
}

void set_ld(const char* prefix, const char* name){
	char str[512];
	str[0] = 0;
	if(prefix != NULL){
		strcat(str, prefix);
		strcat(str, "-none-elf-");
	}
	strcat(str, name);
	if(have_program(str)){
		if(ld != NULL) free(ld);
		ld = malloc(strlen(str) + 1);
		strcpy(ld, str);
	}
}

void set_ar(const char* prefix, const char* name){
	char str[512];
	str[0] = 0;
	if(prefix != NULL){
		strcat(str, prefix);
		strcat(str, "-none-elf-");
	}
	strcat(str, name);
	if(have_program(str)){
		if(ar != NULL) free(ar);
		ar = malloc(strlen(str) + 1);
		strcpy(ar, str);
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

void add_defines(const char* str){
	char* def = malloc(2 + strlen(str) + 1);
	def[0] = 0;
	strcat(def, "-D");
	strcat(def, str);
	if(defines == NULL){
		defines = malloc(strlen(def) + 1);
		strcpy(defines, def);
	}else{
		char* new = malloc(strlen(defines) + 1 + strlen(def) + 1);
		strcpy(new, defines);
		new[strlen(defines)] = ' ';
		strcpy(new + strlen(defines) + 1, def);
		free(defines);
		defines = new;
	}
	free(def);
}

void add_controller(const char* name){
	const char* path = "../dri/controller/";
	char* str = malloc(strlen(path) + strlen(name) + 2 + 1);
	str[0] = 0;
	strcat(str, path);
	strcat(str, name);
	strcat(str, ".c");
	if(drivers == NULL){
		drivers = malloc(strlen(str) + 1);
		strcpy(drivers, str);
	}else{
		char* new = malloc(strlen(drivers) + 1 + strlen(str) + 1);
		strcpy(new, drivers);
		new[strlen(drivers)] = ' ';
		strcpy(new + strlen(drivers) + 1, str);
		free(drivers);
		drivers = new;
	}
	free(str);
	path = "dri/ctrl_";
	str = malloc(strlen(path) + strlen(name) + 2 + 1);
	str[0] = 0;
	strcat(str, path);
	strcat(str, name);
	strcat(str, ".o");
	if(drivers_o == NULL){
		drivers_o = malloc(strlen(str) + 1);
		strcpy(drivers_o, str);
	}else{
		char* new = malloc(strlen(drivers_o) + 1 + strlen(str) + 1);
		strcpy(new, drivers_o);
		new[strlen(drivers_o)] = ' ';
		strcpy(new + strlen(drivers_o) + 1, str);
		free(drivers_o);
		drivers_o = new;
	}
	free(str);
}
void add_console(const char* name){
	const char* path = "../dri/console/";
	char* str = malloc(strlen(path) + strlen(name) + 2 + 1);
	str[0] = 0;
	strcat(str, path);
	strcat(str, name);
	strcat(str, ".c");
	if(drivers == NULL){
		drivers = malloc(strlen(str) + 1);
		strcpy(drivers, str);
	}else{
		char* new = malloc(strlen(drivers) + 1 + strlen(str) + 1);
		strcpy(new, drivers);
		new[strlen(drivers)] = ' ';
		strcpy(new + strlen(drivers) + 1, str);
		free(drivers);
		drivers = new;
	}
	free(str);
	path = "dri/cons_";
	str = malloc(strlen(path) + strlen(name) + 2 + 1);
	str[0] = 0;
	strcat(str, path);
	strcat(str, name);
	strcat(str, ".o");
	if(drivers_o == NULL){
		drivers_o = malloc(strlen(str) + 1);
		strcpy(drivers_o, str);
	}else{
		char* new = malloc(strlen(drivers_o) + 1 + strlen(str) + 1);
		strcpy(new, drivers_o);
		new[strlen(drivers_o)] = ' ';
		strcpy(new + strlen(drivers_o) + 1, str);
		free(drivers_o);
		drivers_o = new;
	}
	free(str);
}

void add_asflags(const char* str){
	if(asflags == NULL){
		asflags = malloc(strlen(str) + 1);
		strcpy(asflags, str);
	}else{
		char* new = malloc(strlen(asflags) + 1 + strlen(str) + 1);
		strcpy(new, asflags);
		new[strlen(asflags)] = ' ';
		strcpy(new + strlen(asflags) + 1, str);
		free(asflags);
		asflags = new;
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
