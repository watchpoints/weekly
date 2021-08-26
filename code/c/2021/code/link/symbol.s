	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 11, 0	sdk_version 11, 3
	.globl	_func_symbol                    ## -- Begin function func_symbol
	.p2align	4, 0x90
_func_symbol:                           ## @func_symbol
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	xorl	%eax, %eax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__DATA,__data
	.globl	_symbol                         ## @symbol
	.p2align	2
_symbol:
	.long	1024                            ## 0x400

.subsections_via_symbols
