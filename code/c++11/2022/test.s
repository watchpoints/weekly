	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 11, 0	sdk_version 12, 1
	.globl	__Z5test1v                      ## -- Begin function _Z5test1v
	.p2align	4, 0x90
__Z5test1v:                             ## @_Z5test1v
Lfunc_begin0:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception0
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$80, %rsp
	movl	$5, -12(%rbp)
	leaq	-8(%rbp), %rdi
	movq	%rdi, -72(%rbp)                 ## 8-byte Spill
	leaq	-12(%rbp), %rsi
	callq	__ZNSt3__1L11make_uniqueIiJiEEENS_11__unique_ifIT_E15__unique_singleEDpOT0_
	movq	-72(%rbp), %rdi                 ## 8-byte Reload
	callq	__ZNSt3__1L4moveIRNS_10unique_ptrIiNS_14default_deleteIiEEEEEEONS_16remove_referenceIT_E4typeEOS7_
	movq	%rax, %rsi
	leaq	-24(%rbp), %rdi
	movq	%rdi, -64(%rbp)                 ## 8-byte Spill
	callq	__ZNSt3__110unique_ptrIiNS_14default_deleteIiEEEC1EOS3_
	movq	-64(%rbp), %rdi                 ## 8-byte Reload
Ltmp0:
	callq	__ZNKSt3__110unique_ptrIiNS_14default_deleteIiEEEdeEv
	movq	%rax, %rcx
Ltmp1:
	movq	%rcx, -56(%rbp)                 ## 8-byte Spill
	jmp	LBB0_1
LBB0_1:
	movq	-56(%rbp), %rax                 ## 8-byte Reload
	movl	(%rax), %esi
Ltmp2:
	movq	__ZNSt3__14coutE@GOTPCREL(%rip), %rdi
	callq	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEElsEi
	movq	%rax, %rcx
Ltmp3:
	movq	%rcx, -80(%rbp)                 ## 8-byte Spill
	jmp	LBB0_2
LBB0_2:
Ltmp4:
	movq	-80(%rbp), %rdi                 ## 8-byte Reload
	movq	__ZNSt3__14endlIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_@GOTPCREL(%rip), %rsi
	callq	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEElsEPFRS3_S4_E
                                        ## kill: def $rcx killed $rax
Ltmp5:
	jmp	LBB0_3
LBB0_3:
	leaq	-24(%rbp), %rdi
	callq	__ZNSt3__1L4moveIRNS_10unique_ptrIiNS_14default_deleteIiEEEEEEONS_16remove_referenceIT_E4typeEOS7_
	movq	%rax, %rsi
	leaq	-48(%rbp), %rdi
	callq	__ZNSt3__110unique_ptrIiNS_14default_deleteIiEEEC1EOS3_
	leaq	-48(%rbp), %rdi
	callq	__ZNSt3__110unique_ptrIiNS_14default_deleteIiEEED1Ev
	leaq	-24(%rbp), %rdi
	callq	__ZNSt3__110unique_ptrIiNS_14default_deleteIiEEED1Ev
	leaq	-8(%rbp), %rdi
	callq	__ZNSt3__110unique_ptrIiNS_14default_deleteIiEEED1Ev
	addq	$80, %rsp
	popq	%rbp
	retq
LBB0_4:
Ltmp6:
	movq	%rax, %rcx
	movl	%edx, %eax
	movq	%rcx, -32(%rbp)
	movl	%eax, -36(%rbp)
	leaq	-24(%rbp), %rdi
	callq	__ZNSt3__110unique_ptrIiNS_14default_deleteIiEEED1Ev
	leaq	-8(%rbp), %rdi
	callq	__ZNSt3__110unique_ptrIiNS_14default_deleteIiEEED1Ev
## %bb.5:
	movq	-32(%rbp), %rdi
	callq	__Unwind_Resume
Lfunc_end0:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.p2align	2
GCC_except_table0:
Lexception0:
	.byte	255                             ## @LPStart Encoding = omit
	.byte	255                             ## @TType Encoding = omit
	.byte	1                               ## Call site Encoding = uleb128
	.uleb128 Lcst_end0-Lcst_begin0
Lcst_begin0:
	.uleb128 Lfunc_begin0-Lfunc_begin0      ## >> Call Site 1 <<
	.uleb128 Ltmp0-Lfunc_begin0             ##   Call between Lfunc_begin0 and Ltmp0
	.byte	0                               ##     has no landing pad
	.byte	0                               ##   On action: cleanup
	.uleb128 Ltmp0-Lfunc_begin0             ## >> Call Site 2 <<
	.uleb128 Ltmp5-Ltmp0                    ##   Call between Ltmp0 and Ltmp5
	.uleb128 Ltmp6-Lfunc_begin0             ##     jumps to Ltmp6
	.byte	0                               ##   On action: cleanup
	.uleb128 Ltmp5-Lfunc_begin0             ## >> Call Site 3 <<
	.uleb128 Lfunc_end0-Ltmp5               ##   Call between Ltmp5 and Lfunc_end0
	.byte	0                               ##     has no landing pad
	.byte	0                               ##   On action: cleanup
Lcst_end0:
	.p2align	2
                                        ## -- End function
	.section	__TEXT,__text,regular,pure_instructions
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__1L11make_uniqueIiJiEEENS_11__unique_ifIT_E15__unique_singleEDpOT0_
__ZNSt3__1L11make_uniqueIiJiEEENS_11__unique_ifIT_E15__unique_singleEDpOT0_: ## @_ZNSt3__1L11make_uniqueIiJiEEENS_11__unique_ifIT_E15__unique_singleEDpOT0_
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -48(%rbp)                 ## 8-byte Spill
	movq	%rdi, %rax
	movq	%rax, -24(%rbp)                 ## 8-byte Spill
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movl	$4, %edi
	callq	__Znwm
	movq	%rax, -32(%rbp)                 ## 8-byte Spill
	movq	%rax, -40(%rbp)                 ## 8-byte Spill
	movq	-16(%rbp), %rdi
	callq	__ZNSt3__1L7forwardIiEEOT_RNS_16remove_referenceIS1_E4typeE
	movq	-48(%rbp), %rdi                 ## 8-byte Reload
	movq	-40(%rbp), %rsi                 ## 8-byte Reload
	movq	%rax, %rcx
	movq	-32(%rbp), %rax                 ## 8-byte Reload
	movl	(%rcx), %ecx
	movl	%ecx, (%rax)
	callq	__ZNSt3__110unique_ptrIiNS_14default_deleteIiEEEC1ILb1EvEEPi
	movq	-24(%rbp), %rax                 ## 8-byte Reload
	addq	$48, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__1L4moveIRNS_10unique_ptrIiNS_14default_deleteIiEEEEEEONS_16remove_referenceIT_E4typeEOS7_
__ZNSt3__1L4moveIRNS_10unique_ptrIiNS_14default_deleteIiEEEEEEONS_16remove_referenceIT_E4typeEOS7_: ## @_ZNSt3__1L4moveIRNS_10unique_ptrIiNS_14default_deleteIiEEEEEEONS_16remove_referenceIT_E4typeEOS7_
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__110unique_ptrIiNS_14default_deleteIiEEEC1EOS3_
__ZNSt3__110unique_ptrIiNS_14default_deleteIiEEEC1EOS3_: ## @_ZNSt3__110unique_ptrIiNS_14default_deleteIiEEEC1EOS3_
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	callq	__ZNSt3__110unique_ptrIiNS_14default_deleteIiEEEC2EOS3_
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNKSt3__110unique_ptrIiNS_14default_deleteIiEEEdeEv
__ZNKSt3__110unique_ptrIiNS_14default_deleteIiEEEdeEv: ## @_ZNKSt3__110unique_ptrIiNS_14default_deleteIiEEEdeEv
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rdi
	callq	__ZNKSt3__117__compressed_pairIPiNS_14default_deleteIiEEE5firstEv
	movq	(%rax), %rax
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEElsEPFRS3_S4_E
__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEElsEPFRS3_S4_E: ## @_ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEElsEPFRS3_S4_E
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rdi
	callq	*-16(%rbp)
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	__ZNSt3__14endlIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_ ## -- Begin function _ZNSt3__14endlIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_
	.weak_definition	__ZNSt3__14endlIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_
	.p2align	4, 0x90
__ZNSt3__14endlIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_: ## @_ZNSt3__14endlIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, -16(%rbp)                 ## 8-byte Spill
	movq	-8(%rbp), %rdi
	movq	(%rdi), %rax
	movq	-24(%rax), %rax
	addq	%rax, %rdi
	movl	$10, %esi
	callq	__ZNKSt3__19basic_iosIcNS_11char_traitsIcEEE5widenEc
	movq	-16(%rbp), %rdi                 ## 8-byte Reload
	movsbl	%al, %esi
	callq	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE3putEc
	movq	-8(%rbp), %rdi
	callq	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE5flushEv
	movq	-8(%rbp), %rax
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__110unique_ptrIiNS_14default_deleteIiEEED1Ev
__ZNSt3__110unique_ptrIiNS_14default_deleteIiEEED1Ev: ## @_ZNSt3__110unique_ptrIiNS_14default_deleteIiEEED1Ev
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rdi
	callq	__ZNSt3__110unique_ptrIiNS_14default_deleteIiEEED2Ev
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_main                           ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movl	$0, -4(%rbp)
	callq	__Z5test1v
	xorl	%eax, %eax
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__1L7forwardIiEEOT_RNS_16remove_referenceIS1_E4typeE
__ZNSt3__1L7forwardIiEEOT_RNS_16remove_referenceIS1_E4typeE: ## @_ZNSt3__1L7forwardIiEEOT_RNS_16remove_referenceIS1_E4typeE
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__110unique_ptrIiNS_14default_deleteIiEEEC1ILb1EvEEPi
__ZNSt3__110unique_ptrIiNS_14default_deleteIiEEEC1ILb1EvEEPi: ## @_ZNSt3__110unique_ptrIiNS_14default_deleteIiEEEC1ILb1EvEEPi
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	callq	__ZNSt3__110unique_ptrIiNS_14default_deleteIiEEEC2ILb1EvEEPi
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__110unique_ptrIiNS_14default_deleteIiEEEC2ILb1EvEEPi
__ZNSt3__110unique_ptrIiNS_14default_deleteIiEEEC2ILb1EvEEPi: ## @_ZNSt3__110unique_ptrIiNS_14default_deleteIiEEEC2ILb1EvEEPi
Lfunc_begin1:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception1
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rdi
Ltmp7:
	leaq	-16(%rbp), %rsi
	leaq	-24(%rbp), %rdx
	callq	__ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEEC1IRS1_NS_18__default_init_tagEEEOT_OT0_
Ltmp8:
	jmp	LBB11_1
LBB11_1:
	addq	$32, %rsp
	popq	%rbp
	retq
LBB11_2:
Ltmp9:
	movq	%rax, %rdi
	callq	___clang_call_terminate
Lfunc_end1:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.p2align	2
GCC_except_table11:
Lexception1:
	.byte	255                             ## @LPStart Encoding = omit
	.byte	155                             ## @TType Encoding = indirect pcrel sdata4
	.uleb128 Lttbase0-Lttbaseref0
Lttbaseref0:
	.byte	1                               ## Call site Encoding = uleb128
	.uleb128 Lcst_end1-Lcst_begin1
Lcst_begin1:
	.uleb128 Ltmp7-Lfunc_begin1             ## >> Call Site 1 <<
	.uleb128 Ltmp8-Ltmp7                    ##   Call between Ltmp7 and Ltmp8
	.uleb128 Ltmp9-Lfunc_begin1             ##     jumps to Ltmp9
	.byte	1                               ##   On action: 1
Lcst_end1:
	.byte	1                               ## >> Action Record 1 <<
                                        ##   Catch TypeInfo 1
	.byte	0                               ##   No further actions
	.p2align	2
                                        ## >> Catch TypeInfos <<
	.long	0                               ## TypeInfo 1
Lttbase0:
	.p2align	2
                                        ## -- End function
	.section	__TEXT,__text,regular,pure_instructions
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEEC1IRS1_NS_18__default_init_tagEEEOT_OT0_
__ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEEC1IRS1_NS_18__default_init_tagEEEOT_OT0_: ## @_ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEEC1IRS1_NS_18__default_init_tagEEEOT_OT0_
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	callq	__ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEEC2IRS1_NS_18__default_init_tagEEEOT_OT0_
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.private_extern	___clang_call_terminate ## -- Begin function __clang_call_terminate
	.globl	___clang_call_terminate
	.weak_definition	___clang_call_terminate
	.p2align	4, 0x90
___clang_call_terminate:                ## @__clang_call_terminate
## %bb.0:
	pushq	%rax
	callq	___cxa_begin_catch
	callq	__ZSt9terminatev
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEEC2IRS1_NS_18__default_init_tagEEEOT_OT0_
__ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEEC2IRS1_NS_18__default_init_tagEEEOT_OT0_: ## @_ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEEC2IRS1_NS_18__default_init_tagEEEOT_OT0_
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$64, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, -48(%rbp)                 ## 8-byte Spill
	movq	%rax, -56(%rbp)                 ## 8-byte Spill
	movq	-16(%rbp), %rdi
	callq	__ZNSt3__1L7forwardIRPiEEOT_RNS_16remove_referenceIS3_E4typeE
	movq	-56(%rbp), %rdi                 ## 8-byte Reload
	movq	%rax, %rsi
	callq	__ZNSt3__122__compressed_pair_elemIPiLi0ELb0EEC2IRS1_vEEOT_
	movq	-48(%rbp), %rax                 ## 8-byte Reload
	movq	%rax, -40(%rbp)                 ## 8-byte Spill
	movq	-24(%rbp), %rdi
	callq	__ZNSt3__1L7forwardINS_18__default_init_tagEEEOT_RNS_16remove_referenceIS2_E4typeE
	movq	-40(%rbp), %rdi                 ## 8-byte Reload
	callq	__ZNSt3__122__compressed_pair_elemINS_14default_deleteIiEELi1ELb1EEC2ENS_18__default_init_tagE
	addq	$64, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__1L7forwardIRPiEEOT_RNS_16remove_referenceIS3_E4typeE
__ZNSt3__1L7forwardIRPiEEOT_RNS_16remove_referenceIS3_E4typeE: ## @_ZNSt3__1L7forwardIRPiEEOT_RNS_16remove_referenceIS3_E4typeE
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__122__compressed_pair_elemIPiLi0ELb0EEC2IRS1_vEEOT_
__ZNSt3__122__compressed_pair_elemIPiLi0ELb0EEC2IRS1_vEEOT_: ## @_ZNSt3__122__compressed_pair_elemIPiLi0ELb0EEC2IRS1_vEEOT_
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, -24(%rbp)                 ## 8-byte Spill
	movq	-16(%rbp), %rdi
	callq	__ZNSt3__1L7forwardIRPiEEOT_RNS_16remove_referenceIS3_E4typeE
	movq	%rax, %rcx
	movq	-24(%rbp), %rax                 ## 8-byte Reload
	movq	(%rcx), %rcx
	movq	%rcx, (%rax)
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__1L7forwardINS_18__default_init_tagEEEOT_RNS_16remove_referenceIS2_E4typeE
__ZNSt3__1L7forwardINS_18__default_init_tagEEEOT_RNS_16remove_referenceIS2_E4typeE: ## @_ZNSt3__1L7forwardINS_18__default_init_tagEEEOT_RNS_16remove_referenceIS2_E4typeE
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__122__compressed_pair_elemINS_14default_deleteIiEELi1ELb1EEC2ENS_18__default_init_tagE
__ZNSt3__122__compressed_pair_elemINS_14default_deleteIiEELi1ELb1EEC2ENS_18__default_init_tagE: ## @_ZNSt3__122__compressed_pair_elemINS_14default_deleteIiEELi1ELb1EEC2ENS_18__default_init_tagE
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -16(%rbp)
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__110unique_ptrIiNS_14default_deleteIiEEED2Ev
__ZNSt3__110unique_ptrIiNS_14default_deleteIiEEED2Ev: ## @_ZNSt3__110unique_ptrIiNS_14default_deleteIiEEED2Ev
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	xorl	%eax, %eax
	movl	%eax, %esi
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rdi
	callq	__ZNSt3__110unique_ptrIiNS_14default_deleteIiEEE5resetEPi
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__110unique_ptrIiNS_14default_deleteIiEEE5resetEPi
__ZNSt3__110unique_ptrIiNS_14default_deleteIiEEE5resetEPi: ## @_ZNSt3__110unique_ptrIiNS_14default_deleteIiEEE5resetEPi
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rdi
	movq	%rdi, -40(%rbp)                 ## 8-byte Spill
	callq	__ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEE5firstEv
	movq	-40(%rbp), %rdi                 ## 8-byte Reload
	movq	(%rax), %rax
	movq	%rax, -24(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, -32(%rbp)                 ## 8-byte Spill
	callq	__ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEE5firstEv
	movq	-32(%rbp), %rcx                 ## 8-byte Reload
	movq	%rcx, (%rax)
	cmpq	$0, -24(%rbp)
	je	LBB20_2
## %bb.1:
	movq	-40(%rbp), %rdi                 ## 8-byte Reload
	callq	__ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEE6secondEv
	movq	%rax, %rdi
	movq	-24(%rbp), %rsi
	callq	__ZNKSt3__114default_deleteIiEclEPi
LBB20_2:
	addq	$48, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEE5firstEv
__ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEE5firstEv: ## @_ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEE5firstEv
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rdi
	callq	__ZNSt3__122__compressed_pair_elemIPiLi0ELb0EE5__getEv
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEE6secondEv
__ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEE6secondEv: ## @_ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEE6secondEv
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rdi
	callq	__ZNSt3__122__compressed_pair_elemINS_14default_deleteIiEELi1ELb1EE5__getEv
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNKSt3__114default_deleteIiEclEPi
__ZNKSt3__114default_deleteIiEclEPi:    ## @_ZNKSt3__114default_deleteIiEclEPi
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	%rax, -24(%rbp)                 ## 8-byte Spill
	cmpq	$0, %rax
	je	LBB23_2
## %bb.1:
	movq	-24(%rbp), %rdi                 ## 8-byte Reload
	callq	__ZdlPv
LBB23_2:
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__122__compressed_pair_elemIPiLi0ELb0EE5__getEv
__ZNSt3__122__compressed_pair_elemIPiLi0ELb0EE5__getEv: ## @_ZNSt3__122__compressed_pair_elemIPiLi0ELb0EE5__getEv
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__122__compressed_pair_elemINS_14default_deleteIiEELi1ELb1EE5__getEv
__ZNSt3__122__compressed_pair_elemINS_14default_deleteIiEELi1ELb1EE5__getEv: ## @_ZNSt3__122__compressed_pair_elemINS_14default_deleteIiEELi1ELb1EE5__getEv
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__110unique_ptrIiNS_14default_deleteIiEEEC2EOS3_
__ZNSt3__110unique_ptrIiNS_14default_deleteIiEEEC2EOS3_: ## @_ZNSt3__110unique_ptrIiNS_14default_deleteIiEEEC2EOS3_
Lfunc_begin2:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception2
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, -32(%rbp)                 ## 8-byte Spill
	movq	-16(%rbp), %rdi
	callq	__ZNSt3__110unique_ptrIiNS_14default_deleteIiEEE7releaseEv
	movq	%rax, -24(%rbp)
	movq	-16(%rbp), %rdi
	callq	__ZNSt3__110unique_ptrIiNS_14default_deleteIiEEE11get_deleterEv
	movq	%rax, %rdi
	callq	__ZNSt3__1L7forwardINS_14default_deleteIiEEEEOT_RNS_16remove_referenceIS3_E4typeE
	movq	-32(%rbp), %rdi                 ## 8-byte Reload
	movq	%rax, %rdx
Ltmp10:
	leaq	-24(%rbp), %rsi
	callq	__ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEEC1IS1_S3_EEOT_OT0_
Ltmp11:
	jmp	LBB26_1
LBB26_1:
	addq	$32, %rsp
	popq	%rbp
	retq
LBB26_2:
Ltmp12:
	movq	%rax, %rdi
	callq	___clang_call_terminate
Lfunc_end2:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.p2align	2
GCC_except_table26:
Lexception2:
	.byte	255                             ## @LPStart Encoding = omit
	.byte	155                             ## @TType Encoding = indirect pcrel sdata4
	.uleb128 Lttbase1-Lttbaseref1
Lttbaseref1:
	.byte	1                               ## Call site Encoding = uleb128
	.uleb128 Lcst_end2-Lcst_begin2
Lcst_begin2:
	.uleb128 Ltmp10-Lfunc_begin2            ## >> Call Site 1 <<
	.uleb128 Ltmp11-Ltmp10                  ##   Call between Ltmp10 and Ltmp11
	.uleb128 Ltmp12-Lfunc_begin2            ##     jumps to Ltmp12
	.byte	1                               ##   On action: 1
Lcst_end2:
	.byte	1                               ## >> Action Record 1 <<
                                        ##   Catch TypeInfo 1
	.byte	0                               ##   No further actions
	.p2align	2
                                        ## >> Catch TypeInfos <<
	.long	0                               ## TypeInfo 1
Lttbase1:
	.p2align	2
                                        ## -- End function
	.section	__TEXT,__text,regular,pure_instructions
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__110unique_ptrIiNS_14default_deleteIiEEE7releaseEv
__ZNSt3__110unique_ptrIiNS_14default_deleteIiEEE7releaseEv: ## @_ZNSt3__110unique_ptrIiNS_14default_deleteIiEEE7releaseEv
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rdi
	movq	%rdi, -24(%rbp)                 ## 8-byte Spill
	callq	__ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEE5firstEv
	movq	-24(%rbp), %rdi                 ## 8-byte Reload
	movq	(%rax), %rax
	movq	%rax, -16(%rbp)
	callq	__ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEE5firstEv
	movq	$0, (%rax)
	movq	-16(%rbp), %rax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__1L7forwardINS_14default_deleteIiEEEEOT_RNS_16remove_referenceIS3_E4typeE
__ZNSt3__1L7forwardINS_14default_deleteIiEEEEOT_RNS_16remove_referenceIS3_E4typeE: ## @_ZNSt3__1L7forwardINS_14default_deleteIiEEEEOT_RNS_16remove_referenceIS3_E4typeE
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__110unique_ptrIiNS_14default_deleteIiEEE11get_deleterEv
__ZNSt3__110unique_ptrIiNS_14default_deleteIiEEE11get_deleterEv: ## @_ZNSt3__110unique_ptrIiNS_14default_deleteIiEEE11get_deleterEv
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rdi
	callq	__ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEE6secondEv
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEEC1IS1_S3_EEOT_OT0_
__ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEEC1IS1_S3_EEOT_OT0_: ## @_ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEEC1IS1_S3_EEOT_OT0_
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	callq	__ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEEC2IS1_S3_EEOT_OT0_
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEEC2IS1_S3_EEOT_OT0_
__ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEEC2IS1_S3_EEOT_OT0_: ## @_ZNSt3__117__compressed_pairIPiNS_14default_deleteIiEEEC2IS1_S3_EEOT_OT0_
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, -40(%rbp)                 ## 8-byte Spill
	movq	%rax, -48(%rbp)                 ## 8-byte Spill
	movq	-16(%rbp), %rdi
	callq	__ZNSt3__1L7forwardIPiEEOT_RNS_16remove_referenceIS2_E4typeE
	movq	-48(%rbp), %rdi                 ## 8-byte Reload
	movq	%rax, %rsi
	callq	__ZNSt3__122__compressed_pair_elemIPiLi0ELb0EEC2IS1_vEEOT_
	movq	-40(%rbp), %rax                 ## 8-byte Reload
	movq	%rax, -32(%rbp)                 ## 8-byte Spill
	movq	-24(%rbp), %rdi
	callq	__ZNSt3__1L7forwardINS_14default_deleteIiEEEEOT_RNS_16remove_referenceIS3_E4typeE
	movq	-32(%rbp), %rdi                 ## 8-byte Reload
	movq	%rax, %rsi
	callq	__ZNSt3__122__compressed_pair_elemINS_14default_deleteIiEELi1ELb1EEC2IS2_vEEOT_
	addq	$48, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__1L7forwardIPiEEOT_RNS_16remove_referenceIS2_E4typeE
__ZNSt3__1L7forwardIPiEEOT_RNS_16remove_referenceIS2_E4typeE: ## @_ZNSt3__1L7forwardIPiEEOT_RNS_16remove_referenceIS2_E4typeE
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__122__compressed_pair_elemIPiLi0ELb0EEC2IS1_vEEOT_
__ZNSt3__122__compressed_pair_elemIPiLi0ELb0EEC2IS1_vEEOT_: ## @_ZNSt3__122__compressed_pair_elemIPiLi0ELb0EEC2IS1_vEEOT_
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, -24(%rbp)                 ## 8-byte Spill
	movq	-16(%rbp), %rdi
	callq	__ZNSt3__1L7forwardIPiEEOT_RNS_16remove_referenceIS2_E4typeE
	movq	%rax, %rcx
	movq	-24(%rbp), %rax                 ## 8-byte Reload
	movq	(%rcx), %rcx
	movq	%rcx, (%rax)
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__122__compressed_pair_elemINS_14default_deleteIiEELi1ELb1EEC2IS2_vEEOT_
__ZNSt3__122__compressed_pair_elemINS_14default_deleteIiEELi1ELb1EEC2IS2_vEEOT_: ## @_ZNSt3__122__compressed_pair_elemINS_14default_deleteIiEELi1ELb1EEC2IS2_vEEOT_
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-16(%rbp), %rdi
	callq	__ZNSt3__1L7forwardINS_14default_deleteIiEEEEOT_RNS_16remove_referenceIS3_E4typeE
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNKSt3__117__compressed_pairIPiNS_14default_deleteIiEEE5firstEv
__ZNKSt3__117__compressed_pairIPiNS_14default_deleteIiEEE5firstEv: ## @_ZNKSt3__117__compressed_pairIPiNS_14default_deleteIiEEE5firstEv
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rdi
	callq	__ZNKSt3__122__compressed_pair_elemIPiLi0ELb0EE5__getEv
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNKSt3__122__compressed_pair_elemIPiLi0ELb0EE5__getEv
__ZNKSt3__122__compressed_pair_elemIPiLi0ELb0EE5__getEv: ## @_ZNKSt3__122__compressed_pair_elemIPiLi0ELb0EE5__getEv
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNKSt3__19basic_iosIcNS_11char_traitsIcEEE5widenEc
__ZNKSt3__19basic_iosIcNS_11char_traitsIcEEE5widenEc: ## @_ZNKSt3__19basic_iosIcNS_11char_traitsIcEEE5widenEc
Lfunc_begin3:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception3
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$64, %rsp
	movb	%sil, %al
	movq	%rdi, -8(%rbp)
	movb	%al, -9(%rbp)
	movq	-8(%rbp), %rsi
	leaq	-24(%rbp), %rdi
	movq	%rdi, -56(%rbp)                 ## 8-byte Spill
	callq	__ZNKSt3__18ios_base6getlocEv
	movq	-56(%rbp), %rdi                 ## 8-byte Reload
Ltmp13:
	callq	__ZNSt3__1L9use_facetINS_5ctypeIcEEEERKT_RKNS_6localeE
	movq	%rax, %rcx
Ltmp14:
	movq	%rcx, -48(%rbp)                 ## 8-byte Spill
	jmp	LBB37_1
LBB37_1:
	movq	-48(%rbp), %rdi                 ## 8-byte Reload
	movsbl	-9(%rbp), %esi
Ltmp15:
	callq	__ZNKSt3__15ctypeIcE5widenEc
	movb	%al, %cl
Ltmp16:
	movb	%cl, -57(%rbp)                  ## 1-byte Spill
	jmp	LBB37_2
LBB37_2:
	leaq	-24(%rbp), %rdi
	callq	__ZNSt3__16localeD1Ev
	movb	-57(%rbp), %al                  ## 1-byte Reload
	movsbl	%al, %eax
	addq	$64, %rsp
	popq	%rbp
	retq
LBB37_3:
Ltmp17:
	movq	%rax, %rcx
	movl	%edx, %eax
	movq	%rcx, -32(%rbp)
	movl	%eax, -36(%rbp)
	leaq	-24(%rbp), %rdi
	callq	__ZNSt3__16localeD1Ev
## %bb.4:
	movq	-32(%rbp), %rdi
	callq	__Unwind_Resume
Lfunc_end3:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.p2align	2
GCC_except_table37:
Lexception3:
	.byte	255                             ## @LPStart Encoding = omit
	.byte	255                             ## @TType Encoding = omit
	.byte	1                               ## Call site Encoding = uleb128
	.uleb128 Lcst_end3-Lcst_begin3
Lcst_begin3:
	.uleb128 Lfunc_begin3-Lfunc_begin3      ## >> Call Site 1 <<
	.uleb128 Ltmp13-Lfunc_begin3            ##   Call between Lfunc_begin3 and Ltmp13
	.byte	0                               ##     has no landing pad
	.byte	0                               ##   On action: cleanup
	.uleb128 Ltmp13-Lfunc_begin3            ## >> Call Site 2 <<
	.uleb128 Ltmp16-Ltmp13                  ##   Call between Ltmp13 and Ltmp16
	.uleb128 Ltmp17-Lfunc_begin3            ##     jumps to Ltmp17
	.byte	0                               ##   On action: cleanup
	.uleb128 Ltmp16-Lfunc_begin3            ## >> Call Site 3 <<
	.uleb128 Lfunc_end3-Ltmp16              ##   Call between Ltmp16 and Lfunc_end3
	.byte	0                               ##     has no landing pad
	.byte	0                               ##   On action: cleanup
Lcst_end3:
	.p2align	2
                                        ## -- End function
	.section	__TEXT,__text,regular,pure_instructions
	.p2align	4, 0x90                         ## -- Begin function _ZNSt3__1L9use_facetINS_5ctypeIcEEEERKT_RKNS_6localeE
__ZNSt3__1L9use_facetINS_5ctypeIcEEEERKT_RKNS_6localeE: ## @_ZNSt3__1L9use_facetINS_5ctypeIcEEEERKT_RKNS_6localeE
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	__ZNSt3__15ctypeIcE2idE@GOTPCREL(%rip), %rsi
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rdi
	callq	__ZNKSt3__16locale9use_facetERNS0_2idE
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.p2align	4, 0x90                         ## -- Begin function _ZNKSt3__15ctypeIcE5widenEc
__ZNKSt3__15ctypeIcE5widenEc:           ## @_ZNKSt3__15ctypeIcE5widenEc
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movb	%sil, %al
	movq	%rdi, -8(%rbp)
	movb	%al, -9(%rbp)
	movq	-8(%rbp), %rdi
	movb	-9(%rbp), %cl
	movq	(%rdi), %rax
	movsbl	%cl, %esi
	callq	*56(%rax)
	movsbl	%al, %eax
	addq	$16, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
.subsections_via_symbols
