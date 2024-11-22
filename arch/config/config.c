/* $Id$ */

#include "parse.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>

extern FILE* yyin;
extern int yyparse(void);

#define CREATE(x) \
	printf("creating " x "\n"); \
	f = fopen(x, "w"); \
	if(f == NULL){ \
		fprintf(stderr, "failed to open\n"); \
		return 1; \
	}

int main(int argc, char** argv){
	char buffer[PATH_MAX + 1 + 6 + 5 + 1];
	FILE* f;
	if(argc != 2){
		fprintf(stderr, "usage: %s file\n", argv[0]);
		return 1;
	}
	yyin = fopen(argv[1], "r");
	if(yyin == NULL){
		fprintf(stderr, "could not open the file\n");
		return 1;
	}
	if(yyparse()){
		return 1;
	}
	fclose(yyin);
	while(1){
		getcwd(buffer, PATH_MAX);
		if(strcmp(buffer, "/") == 0){
			fprintf(stderr, "README.ROOT not found, your distribution might be broken\n");
			return 1;
		}
		strcat(buffer, "/README.ROOT");
		if(access(buffer, F_OK) == 0){
			getcwd(buffer, PATH_MAX);
			break;
		}
		chdir("..");
	}
	if(cc == NULL){
		fprintf(stderr, "failed to probe C compiler\n");
		return 1;
	}
	printf("C compiler: %s\n", cc);
	printf("CFLAGS: %s\n", cflags == NULL ? "" : cflags);
	printf("LDFLAGS: %s\n", ldflags == NULL ? "" : ldflags);
	printf("root directory: %s\n", buffer);
	mkdir("build", 0755);
	if(chdir("build")){
		fprintf(stderr, "chdir failure\n");
		return 1;
	}

	CREATE("config.mk");
	fprintf(f, "CC = %s\n", cc);
	fprintf(f, "CFLAGS = %s\n", cflags == NULL ? "" : cflags);
	fprintf(f, "LDFLAGS = %s\n", ldflags == NULL ? "" : ldflags);
	fclose(f);

	CREATE("Makefile");
	fclose(f);

	printf("run `make' in `%s/build' to build the kernel\n", buffer);
	return 0;
}
