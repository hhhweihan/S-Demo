	.file	"fork_test.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"p = %c , number = %d \n "
	.text
	.globl	main
	.type	main, @function
main:
.LFB60:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	call	fork@PLT
	testl	%eax, %eax
	jne	.L2
	movl	$11, %ecx
	movl	$99, %edx
	leaq	.LC0(%rip), %rsi
	movl	$1, %edi
	call	__printf_chk@PLT
	movl	$0, %edi
	call	exit@PLT
.L2:
	movl	$14, %ecx
	movl	$112, %edx
	leaq	.LC0(%rip), %rsi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	movl	$0, %edi
	call	exit@PLT
	.cfi_endproc
.LFE60:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
