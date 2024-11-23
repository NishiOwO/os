/* $Id$ */

.set MAGIC_NUMBER,	0x1badb002
.set FLAGS,		(1 << 0) | (1 << 1)
.set CHECKSUM,		-(MAGIC_NUMBER + FLAGS)

.align 8
.section .multiboot
	.long MAGIC_NUMBER
	.long FLAGS
	.long CHECKSUM

.section .data
	seax: .long 0
	sebx: .long 0

.section .bss
	.align 16
	stack_bottom:
	.skip 16384
	stack_top:

.section .text
.global _start
.extern init_kernel
_start:
	mov %eax, seax
	mov %ebx, sebx
	mov $stack_top, %esp
	mov seax, %eax
	mov sebx, %ebx
	push %ebx
	push %eax
	call init_kernel
.end:
