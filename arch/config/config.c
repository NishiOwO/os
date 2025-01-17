/* $Id$ */

#include "parse.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
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
	char* sources[256];
	int count = 0;
	char located[PATH_MAX + 1];
	char old[PATH_MAX + 1];
	int i;
	DIR* dir;
	if(argc != 2){
		fprintf(stderr, "usage: %s file\n", argv[0]);
		return 1;
	}
	strcpy(located, argv[1]);
	for(i = strlen(located) - 1; i >= 0; i--){
		if(located[i] == '/'){
			break;
		}
		located[i] = 0;
	}
	if(strlen(located) == 0){
		strcpy(located, "./");
	}
	yyin = fopen(argv[1], "r");
	if(yyin == NULL){
		fprintf(stderr, "could not open the file\n");
		return 1;
	}
	printf("config located in: %s\n", located);
	if(yyparse()){
		return 1;
	}
	fclose(yyin);
	getcwd(old, PATH_MAX);

	chdir(located);
	chdir("..");

	dir = opendir(".");
	if(dir != NULL){
		struct dirent* d;
		while((d = readdir(dir)) != NULL){
			int cond = 0;
			if(strcmp(d->d_name, "..") == 0 || strcmp(d->d_name, ".") == 0) continue;
			cond = cond || (strlen(d->d_name) >= 2 && (d->d_name[strlen(d->d_name) - 1] == 's' || d->d_name[strlen(d->d_name) - 1] == 'c' || d->d_name[strlen(d->d_name) - 1] == 'h') && d->d_name[strlen(d->d_name) - 2] == '.');
			if(cond){
				char* path = malloc(strlen(d->d_name) + 1);
				strcpy(path, d->d_name);
				sources[count++] = path;
			}
		}
		closedir(dir);
	}
	getcwd(located, PATH_MAX);

	chdir(old);
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
	if(cc == NULL) cc = getenv("CC");
	if(as == NULL) as = getenv("AS");
	if(ld == NULL) ld = getenv("LD");
	if(ar == NULL) ar = getenv("AR");
	if(cpp == NULL) cpp = getenv("CPP");
	if(cc == NULL){
		fprintf(stderr, "failed to probe C compiler\n");
		return 1;
	}
	if(as == NULL){
		fprintf(stderr, "failed to probe assembler\n");
		return 1;
	}
	if(ld == NULL){
		fprintf(stderr, "failed to probe linker\n");
		return 1;
	}
	if(ar == NULL){
		fprintf(stderr, "failed to probe archiver\n");
		return 1;
	}
	if(cpp == NULL){
		fprintf(stderr, "failed to probe preprocessor\n");
		return 1;
	}
	printf("C compiler: %s\n", cc);
	printf("assembler: %s\n", as);
	printf("linker: %s\n", ld);
	printf("archiver: %s\n", ar);
	printf("preprocessor: %s\n", cpp);
	printf("DEFINES: %s\n", defines == NULL ? "" : defines);
	printf("CFLAGS: %s\n", cflags == NULL ? "" : cflags);
	printf("ASFLAGS: %s\n", asflags == NULL ? "" : asflags);
	printf("LDFLAGS: %s\n", ldflags == NULL ? "" : ldflags);
	printf("root directory: %s\n", buffer);
	mkdir("build", 0755);
	if(chdir("build")){
		fprintf(stderr, "chdir failure\n");
		return 1;
	}
	mkdir("arch", 0755);
	mkdir("rootfs", 0755);
	mkdir("dri", 0755);

	CREATE("config.mk");
	fprintf(f, "BUILDDIR = %s/build\n", buffer);
	fprintf(f, "TOPDIR = %s\n", buffer);
	fprintf(f, "CPP = %s -P\n", cpp);
	fprintf(f, "CC = %s\n", cc);
	fprintf(f, "AS = %s\n", as);
	fprintf(f, "LD = %s\n", ld);
	fprintf(f, "AR = %s\n", ar);
	fprintf(f, "DEFINES = %s\n", defines == NULL ? "" : defines);
	fprintf(f, "CFLAGS = %s -I$(BUILDDIR) -I$(TOPDIR) $(DEFINES) -ffreestanding\n", cflags == NULL ? "" : cflags);
	fprintf(f, "ASFLAGS = %s\n", asflags == NULL ? "" : asflags);
	fprintf(f, "LDFLAGS = %s\n", ldflags == NULL ? "" : ldflags);
	fclose(f);

	CREATE("Makefile");
	fprintf(f, "include config.mk\n");
	fprintf(f, ".PHONY: all clean arch ../kern ../c\n");
	fprintf(f, ".SUFFIXES: .c .s .o\n");
	fprintf(f, "all: iskra$(BOOTLOADER).iso\n");
	fprintf(f, "iskra.iso: rootfs/kernel ../contrib/boot/boot.cfg\n");
	fprintf(f, "	cp ../contrib/boot/* ./rootfs/\n");
	fprintf(f, "	mkisofs -R -o $@ -uid 0 -gid 0 -no-emul-boot -b cdboot rootfs\n");
	fprintf(f, "iskra-grub.iso: rootfs/kernel ../contrib/grub/grub.cfg\n");
	fprintf(f, "	mkdir -p ./rootfs/boot/grub\n");
	fprintf(f, "	cp ../contrib/grub/grub.cfg ./rootfs/boot/grub/\n");
	fprintf(f, "	grub-mkrescue -o $@ ./rootfs\n");
	fprintf(f, "rootfs/kernel: arch ../kern ../c linker.ld %s drivers.o\n", drivers_o == NULL ? "" : drivers_o);
	fprintf(f, "	$(LD) -Tlinker.ld $(LDFLAGS) -o $@ arch/*.o kern/*.o %s drivers.o c/libc.a\n", drivers_o == NULL ? "" : drivers_o);
	fprintf(f, "arch::\n");
	fprintf(f, "	$(MAKE) -C $@\n");
	fprintf(f, "../kern::\n");
	fprintf(f, "	mkdir -p ./kern\n");
	fprintf(f, "	$(MAKE) -C $@\n");
	fprintf(f, "../c::\n");
	fprintf(f, "	mkdir -p ./c\n");
	fprintf(f, "	$(MAKE) -C $@\n");
	fprintf(f, "linker.ld: ../link/linker.ld\n");
	fprintf(f, "	$(CPP) $(DEFINES) ../link/linker.ld > $@\n");
	fprintf(f, "clean:\n");
	fprintf(f, "	-rm -f kernel *.iso %s drivers.o\n", drivers_o == NULL ? "" : drivers_o);
	fprintf(f, "	$(MAKE) -C arch clean\n");
	fprintf(f, "	$(MAKE) -C ../kern clean\n");
	fprintf(f, "drivers.o: drivers.c\n");
	fprintf(f, "	$(CC) $(CFLAGS) -c -o $@ drivers.c\n");

	if(drivers != NULL){
		while(1){
			int i;
			int brk = 0;
			char* c;
			char* o;
			for(i = 0;; i++){
				if(drivers[i] == ' ' || drivers[i] == 0){
					if(drivers[i] == 0) brk = 1;
					drivers[i] = 0;
					c = drivers;
					drivers += i + 1;
					break;
				}
			}
			for(i = 0;; i++){
				if(drivers_o[i] == ' ' || drivers_o[i] == 0){
					if(drivers_o[i] == 0) brk = 1;
					drivers_o[i] = 0;
					o = drivers_o;
					drivers_o += i + 1;
					break;
				}
			}
			fprintf(f, "%s: %s\n", o, c);
			fprintf(f, "	$(CC) $(CFLAGS) -c -o $@ %s\n", c);
			if(brk) break;
		}
	}
	fclose(f);

	CREATE("arch/Makefile");
	fprintf(f, "include ../config.mk\n");
	fprintf(f, ".PHONY: all clean\n");
	fprintf(f, ".SUFFIXES: .c .s .o\n");
	fprintf(f, "all:");
	for(i = 0; i < count; i++){
		char* name = malloc(strlen(sources[i]) + 1);
		int j;
		int add = 1;
		strcpy(name, sources[i]);
		for(j = strlen(name) - 1; j >= 0; j--){
			if(name[j] == '.'){
				name[j] = 0;
				if(strcmp(name + j + 1, "h") == 0){
					add = 0;
				}
				break;
			}
		}
		if(add){
			fprintf(f, " %s.o", name);
		}
		free(name);
	}
	fprintf(f, "\n");
	fprintf(f, ".s.o:\n");
	fprintf(f, "	$(AS) $(ASFLAGS) -o $@ $<\n");
	fprintf(f, ".c.o:\n");
	fprintf(f, "	$(CC) $(CFLAGS) -c -o $@ $<\n");
	fprintf(f, "clean:\n");
	fprintf(f, "	rm -f *.o\n");
	fclose(f);

	CREATE("drivers.c");
	fprintf(f, "#include <kern/debug.h>\n");
	if(drivernames != NULL){
		char* str = malloc(strlen(drivernames) + 1);
		int i;
		int incr = 0;
		strcpy(str, drivernames);
		for(i = 0;; i++){
			if(str[i] == ' ' || str[i] == 0){
				int brk = str[i] == 0;
				str[i] = 0;
				fprintf(f, "void %s_init(void);\n", str + incr);
				incr = i + 1;
				if(brk) break;
			}
		}
		free(str);
	}
	fprintf(f, "void drivers_init(void){\n");
	fprintf(f, "	kdebug(\"Initializing drivers\");\n");
	if(drivernames != NULL){
		char* str = malloc(strlen(drivernames) + 1);
		int i;
		int incr = 0;
		strcpy(str, drivernames);
		for(i = 0;; i++){
			if(str[i] == ' ' || str[i] == 0){
				int brk = str[i] == 0;
				str[i] = 0;
				fprintf(f, "	%s_init();\n", str + incr);
				incr = i + 1;
				if(brk) break;
			}
		}
		free(str);
	}
	fprintf(f, "}\n");
	fclose(f);

	if(chdir("arch")){
		fprintf(stderr, "chdir failure\n");
		return 1;
	}
	for(i = 0; i < count; i++){
		char src[PATH_MAX + 1];
		FILE* fin;
		FILE* fout;
		char* txtbuf;
		struct stat s;
		sprintf(src, "%s/%s", located, sources[i]);
		stat(src, &s);
		fin = fopen(src, "r");
		fout = fopen(sources[i], "w");
		txtbuf = malloc(s.st_size);
		printf("copying %s\n", sources[i]);
		fread(txtbuf, 1, s.st_size, fin);
		fwrite(txtbuf, 1, s.st_size, fout);
		fclose(fout);
		fclose(fin);
	}
	if(chdir("..")){
		fprintf(stderr, "chdir failure\n");
		return 1;
	}

	printf("run `make' in `%s/build' to build the kernel\n", buffer);
	return 0;
}
