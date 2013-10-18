	.file	"simulazione.c"
	.text
	.p2align 4,,15
	.type	cmpint, @function
cmpint:
.LFB624:
	.cfi_startproc
	movl	(%rdi), %eax
	subl	(%rsi), %eax
	ret
	.cfi_endproc
.LFE624:
	.size	cmpint, .-cmpint
	.p2align 4,,15
	.type	load_configuration_null, @function
load_configuration_null:
.LFB632:
	.cfi_startproc
	movss	lambda(%rip), %xmm2
	movss	.LC0(%rip), %xmm1
	movaps	%xmm2, %xmm4
	movss	sigma(%rip), %xmm3
	movss	alfa_uniform(%rip), %xmm0
	mulss	%xmm1, %xmm4
	movq	dots(%rip), %rdx
	addss	%xmm3, %xmm0
	movaps	%xmm4, %xmm5
	addss	%xmm0, %xmm5
	comiss	%xmm5, %xmm2
	jbe	.L31
	movss	.LC1(%rip), %xmm4
	xorps	%xmm5, %xmm4
.L5:
	movl	N(%rip), %eax
	movq	dots+8(%rip), %rdi
	movq	dots+16(%rip), %rsi
	cvtsi2ss	%eax, %xmm6
	cmpl	$1, %eax
	mulss	.LC2(%rip), %xmm6
	mulss	%xmm4, %xmm6
	movss	%xmm6, (%rdx)
	movl	$0x00000000, (%rdi)
	movl	$0x00000000, (%rsi)
	jle	.L2
	movss	lambda(%rip), %xmm4
	subl	$2, %eax
	leaq	4(,%rax,4), %r8
	movq	%rax, %rcx
	movss	(%rdx), %xmm6
	movaps	%xmm4, %xmm5
	andl	$3, %ecx
	mulss	%xmm1, %xmm5
	movaps	%xmm5, %xmm2
	addss	%xmm0, %xmm2
	comiss	%xmm2, %xmm4
	jbe	.L35
.L11:
	addss	%xmm6, %xmm2
	movl	$4, %eax
	cmpq	%r8, %rax
	movss	%xmm2, 4(%rdx)
	movl	$0x00000000, 4(%rdi)
	movl	$0x00000000, 4(%rsi)
	je	.L2
	testq	%rcx, %rcx
	je	.L8
	cmpq	$1, %rcx
	je	.L29
	cmpq	$2, %rcx
	je	.L30
	movss	lambda(%rip), %xmm9
	movss	(%rdx,%rax), %xmm6
	movaps	%xmm9, %xmm5
	mulss	%xmm1, %xmm5
	movaps	%xmm5, %xmm2
	addss	%xmm0, %xmm2
	comiss	%xmm2, %xmm9
	jbe	.L36
.L13:
	addss	%xmm6, %xmm2
	movss	%xmm2, 4(%rdx,%rax)
	movl	$0x00000000, 4(%rdi,%rax)
	movl	$0x00000000, 4(%rsi,%rax)
	addq	$4, %rax
.L30:
	movss	lambda(%rip), %xmm10
	movss	(%rdx,%rax), %xmm6
	movaps	%xmm10, %xmm5
	mulss	%xmm1, %xmm5
	movaps	%xmm5, %xmm2
	addss	%xmm0, %xmm2
	comiss	%xmm2, %xmm10
	jbe	.L37
.L16:
	addss	%xmm6, %xmm2
	movss	%xmm2, 4(%rdx,%rax)
	movl	$0x00000000, 4(%rdi,%rax)
	movl	$0x00000000, 4(%rsi,%rax)
	addq	$4, %rax
.L29:
	movss	lambda(%rip), %xmm11
	movss	(%rdx,%rax), %xmm6
	movaps	%xmm11, %xmm5
	mulss	%xmm1, %xmm5
	movaps	%xmm5, %xmm2
	addss	%xmm0, %xmm2
	comiss	%xmm2, %xmm11
	jbe	.L38
.L19:
	addss	%xmm6, %xmm2
	movss	%xmm2, 4(%rdx,%rax)
	movl	$0x00000000, 4(%rdi,%rax)
	movl	$0x00000000, 4(%rsi,%rax)
	addq	$4, %rax
	cmpq	%r8, %rax
	je	.L39
.L8:
	movss	lambda(%rip), %xmm7
	movss	(%rdx,%rax), %xmm6
	movaps	%xmm7, %xmm5
	mulss	%xmm1, %xmm5
	movaps	%xmm5, %xmm2
	addss	%xmm0, %xmm2
	comiss	%xmm2, %xmm7
	ja	.L7
	movaps	%xmm5, %xmm2
	addss	%xmm3, %xmm2
.L7:
	addss	%xmm6, %xmm2
	leaq	4(%rax), %rcx
	movss	%xmm2, 4(%rdx,%rax)
	movl	$0x00000000, 4(%rdi,%rax)
	movl	$0x00000000, 4(%rsi,%rax)
	movss	lambda(%rip), %xmm8
	movss	4(%rdx,%rax), %xmm6
	movaps	%xmm8, %xmm5
	mulss	%xmm1, %xmm5
	movaps	%xmm5, %xmm2
	addss	%xmm0, %xmm2
	comiss	%xmm2, %xmm8
	ja	.L22
	movaps	%xmm5, %xmm2
	addss	%xmm3, %xmm2
.L22:
	addss	%xmm6, %xmm2
	leaq	4(%rcx), %rax
	movss	%xmm2, 4(%rdx,%rcx)
	movl	$0x00000000, 4(%rdi,%rcx)
	movl	$0x00000000, 4(%rsi,%rcx)
	movss	lambda(%rip), %xmm12
	movss	4(%rdx,%rcx), %xmm6
	movaps	%xmm12, %xmm5
	mulss	%xmm1, %xmm5
	movaps	%xmm5, %xmm2
	addss	%xmm0, %xmm2
	comiss	%xmm2, %xmm12
	ja	.L24
	movaps	%xmm5, %xmm2
	addss	%xmm3, %xmm2
.L24:
	addss	%xmm6, %xmm2
	movss	%xmm2, 4(%rdx,%rax)
	movl	$0x00000000, 4(%rdi,%rax)
	movl	$0x00000000, 4(%rsi,%rax)
	leaq	8(%rcx), %rax
	movss	lambda(%rip), %xmm13
	movss	8(%rdx,%rcx), %xmm6
	movaps	%xmm13, %xmm5
	mulss	%xmm1, %xmm5
	movaps	%xmm5, %xmm2
	addss	%xmm0, %xmm2
	comiss	%xmm2, %xmm13
	ja	.L26
	movaps	%xmm5, %xmm2
	addss	%xmm3, %xmm2
.L26:
	addss	%xmm6, %xmm2
	movss	%xmm2, 4(%rdx,%rax)
	movl	$0x00000000, 4(%rdi,%rax)
	movl	$0x00000000, 4(%rsi,%rax)
	leaq	12(%rcx), %rax
	cmpq	%r8, %rax
	jne	.L8
	rep
	ret
	.p2align 4,,10
	.p2align 3
.L2:
	rep
	ret
	.p2align 4,,10
	.p2align 3
.L31:
	addss	%xmm3, %xmm4
	movss	.LC1(%rip), %xmm5
	xorps	%xmm5, %xmm4
	jmp	.L5
	.p2align 4,,10
	.p2align 3
.L35:
	movaps	%xmm5, %xmm2
	addss	%xmm3, %xmm2
	jmp	.L11
	.p2align 4,,10
	.p2align 3
.L38:
	movaps	%xmm5, %xmm2
	addss	%xmm3, %xmm2
	jmp	.L19
	.p2align 4,,10
	.p2align 3
.L37:
	movaps	%xmm5, %xmm2
	addss	%xmm3, %xmm2
	jmp	.L16
	.p2align 4,,10
	.p2align 3
.L36:
	movaps	%xmm5, %xmm2
	addss	%xmm3, %xmm2
	jmp	.L13
	.p2align 4,,10
	.p2align 3
.L39:
	rep
	ret
	.cfi_endproc
.LFE632:
	.size	load_configuration_null, .-load_configuration_null
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC4:
	.string	"I need more memory, please modify constant ODGRMAX in simprivate.h\n"
	.text
	.p2align 4,,15
	.type	load_laplacian_null, @function
load_laplacian_null:
.LFB627:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movq	lpl_index(%rip), %rdi
	movq	lpl(%rip), %rdx
	movl	$0, (%rdi)
	movl	$1, (%rdx)
	movl	N(%rip), %ecx
	cmpl	$2, %ecx
	jle	.L44
	xorl	%esi, %esi
	movl	$1, %eax
.L42:
	leal	2(%rsi), %r10d
	movl	%eax, 4(%rdi,%rsi,4)
	movl	%esi, 4(%rdx,%rsi,8)
	leal	2(%rax), %r8d
	movl	%r10d, 8(%rdx,%rsi,8)
	leaq	1(%rsi), %r9
	movl	%r8d, %eax
	movl	N(%rip), %ecx
	leal	-1(%rcx), %r11d
	cmpl	%r10d, %r11d
	jle	.L60
	leal	2(%r9), %r10d
	movl	%r8d, 4(%rdi,%r9,4)
	addl	$2, %eax
	movl	%r9d, 4(%rdx,%r9,8)
	movl	%r10d, 8(%rdx,%r9,8)
	leaq	2(%rsi), %r9
	movl	N(%rip), %ecx
	leal	-1(%rcx), %r11d
	cmpl	%r10d, %r11d
	jle	.L60
	leal	2(%r9), %r10d
	movl	%eax, 4(%rdi,%r9,4)
	movl	%r9d, 4(%rdx,%r9,8)
	leal	4(%r8), %eax
	movl	%r10d, 8(%rdx,%r9,8)
	leaq	3(%rsi), %r9
	movl	N(%rip), %ecx
	leal	-1(%rcx), %r11d
	cmpl	%r10d, %r11d
	jle	.L60
	leal	2(%r9), %r10d
	movl	%eax, 4(%rdi,%r9,4)
	movl	%r9d, 4(%rdx,%r9,8)
	leal	6(%r8), %eax
	movl	%r10d, 8(%rdx,%r9,8)
	leaq	4(%rsi), %r9
	movl	N(%rip), %ecx
	leal	-1(%rcx), %r11d
	cmpl	%r10d, %r11d
	jle	.L60
	leal	2(%r9), %r10d
	movl	%eax, 4(%rdi,%r9,4)
	movl	%r9d, 4(%rdx,%r9,8)
	leal	8(%r8), %eax
	movl	%r10d, 8(%rdx,%r9,8)
	leaq	5(%rsi), %r9
	movl	N(%rip), %ecx
	leal	-1(%rcx), %r11d
	cmpl	%r10d, %r11d
	jle	.L60
	leal	2(%r9), %r10d
	movl	%eax, 4(%rdi,%r9,4)
	movl	%r9d, 4(%rdx,%r9,8)
	leal	10(%r8), %eax
	movl	%r10d, 8(%rdx,%r9,8)
	leaq	6(%rsi), %r9
	movl	N(%rip), %ecx
	leal	-1(%rcx), %r11d
	cmpl	%r10d, %r11d
	jle	.L60
	leal	2(%r9), %r10d
	movl	%eax, 4(%rdi,%r9,4)
	movl	%r9d, 4(%rdx,%r9,8)
	leal	12(%r8), %eax
	movl	%r10d, 8(%rdx,%r9,8)
	leaq	7(%rsi), %r9
	movl	N(%rip), %ecx
	leal	-1(%rcx), %r11d
	cmpl	%r10d, %r11d
	jle	.L60
	leal	2(%r9), %r10d
	movl	%eax, 4(%rdi,%r9,4)
	addq	$8, %rsi
	movl	%r9d, 4(%rdx,%r9,8)
	leal	14(%r8), %eax
	movl	%r10d, 8(%rdx,%r9,8)
	movl	N(%rip), %ecx
	leal	-1(%rcx), %r8d
	cmpl	%r10d, %r8d
	jg	.L42
	.p2align 4,,10
	.p2align 3
.L60:
	leal	1(%rax), %esi
	movslq	%eax, %r8
	salq	$2, %r8
.L41:
	movslq	%ecx, %r9
	movl	%eax, -4(%rdi,%r9,4)
	movl	N(%rip), %ecx
	subl	$2, %ecx
	movl	%ecx, (%rdx,%r8)
	movslq	N(%rip), %rdx
	movl	%esi, (%rdi,%rdx,4)
	movl	N(%rip), %eax
	addl	%eax, %eax
	cmpl	%esi, %eax
	jl	.L61
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L44:
	.cfi_restore_state
	movl	$2, %esi
	movl	$4, %r8d
	movl	$1, %eax
	jmp	.L41
.L61:
	movl	$.LC4, %edi
	movl	$67, %edx
	movl	$1, %esi
	movq	stderr(%rip), %rcx
	call	fwrite
	movl	$-4, %edi
	call	exit
	.cfi_endproc
.LFE627:
	.size	load_laplacian_null, .-load_laplacian_null
	.section	.rodata.str1.8
	.align 8
.LC5:
	.string	"Wrong initial conditions --> %d %d\n"
	.text
	.p2align 4,,15
	.type	count_contacts, @function
count_contacts:
.LFB635:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$24, %rsp
	.cfi_def_cfa_offset 80
	movl	$0, contacts(%rip)
	movl	$0, contacts_loc(%rip)
	movl	N(%rip), %ebx
	testl	%ebx, %ebx
	jle	.L62
	movss	sigma(%rip), %xmm5
	xorl	%r11d, %r11d
	xorl	%edi, %edi
	movq	dots+16(%rip), %r10
	xorl	%r13d, %r13d
	movl	$1, %r12d
	movss	alfa_uniform(%rip), %xmm4
	movss	alfa_localized(%rip), %xmm3
	addss	%xmm5, %xmm4
	movq	dots+8(%rip), %r9
	addss	%xmm5, %xmm3
	movq	dots(%rip), %r8
	mulss	%xmm5, %xmm5
	movq	locmask(%rip), %rbp
	mulss	%xmm4, %xmm4
	mulss	%xmm3, %xmm3
	.p2align 4,,10
	.p2align 3
.L64:
	testl	%ebx, %ebx
	movl	%edi, %edx
	jle	.L72
	testl	%edi, %edi
	movl	%edi, %ecx
	movl	%r12d, %esi
	leal	3(%rdx), %r14d
	cmovns	%edi, %r14d
	andl	$3, %ecx
	xorl	%eax, %eax
	sall	%cl, %esi
	movl	%esi, 8(%rsp)
	sarl	$2, %r14d
	xorl	%esi, %esi
	leal	-1(%rbx), %ecx
	movslq	%r14d, %r14
	addq	%rbp, %r14
	andl	$1, %ecx
	cmpl	%edi, %esi
	je	.L78
	movss	(%r8,%rdi,4), %xmm7
	movss	(%r9,%rdi,4), %xmm0
	subss	(%r8), %xmm7
	subss	(%r9), %xmm0
	movss	(%r10,%rdi,4), %xmm6
	mulss	%xmm7, %xmm7
	mulss	%xmm0, %xmm0
	subss	(%r10), %xmm6
	mulss	%xmm6, %xmm6
	addss	%xmm7, %xmm0
	addss	%xmm6, %xmm0
	comiss	%xmm0, %xmm5
	ja	.L73
	comiss	%xmm0, %xmm4
	jbe	.L95
	xorl	%eax, %eax
	cmpl	%edi, %esi
	setg	%al
	addl	%eax, %r11d
.L95:
	comiss	%xmm0, %xmm3
	jbe	.L78
	cmpl	%edx, %esi
	jle	.L78
	testb	$1, 0(%rbp)
	je	.L78
	movzbl	(%r14), %r15d
	andl	8(%rsp), %r15d
	cmpl	$1, %r15d
	sbbl	$-1, %r13d
	.p2align 4,,10
	.p2align 3
.L78:
	cmpl	$1, %ebx
	movl	$1, %eax
	jle	.L72
	testl	%ecx, %ecx
	je	.L71
	movl	$1, %esi
	cmpl	%edx, %esi
	je	.L82
	movss	(%r8,%rdi,4), %xmm9
	movss	(%r9,%rdi,4), %xmm0
	subss	4(%r8), %xmm9
	subss	4(%r9), %xmm0
	movss	(%r10,%rdi,4), %xmm8
	mulss	%xmm9, %xmm9
	mulss	%xmm0, %xmm0
	subss	4(%r10), %xmm8
	mulss	%xmm8, %xmm8
	addss	%xmm9, %xmm0
	addss	%xmm8, %xmm0
	comiss	%xmm0, %xmm5
	ja	.L73
	comiss	%xmm0, %xmm4
	jbe	.L96
	movl	$1, %r15d
	xorl	%esi, %esi
	cmpl	%edx, %r15d
	setg	%sil
	addl	%esi, %r11d
.L96:
	comiss	%xmm0, %xmm3
	jbe	.L82
	movl	$1, %ecx
	cmpl	%edx, %ecx
	jle	.L82
	movzbl	0(%rbp), %esi
	leal	(%r12,%r12), %r15d
	testl	%esi, %r15d
	je	.L82
	movzbl	(%r14), %ecx
	andl	8(%rsp), %ecx
	cmpl	$1, %ecx
	sbbl	$-1, %r13d
	.p2align 4,,10
	.p2align 3
.L82:
	leal	1(%rax), %ecx
	addq	$1, %rax
	cmpl	%ebx, %ecx
	jge	.L72
	.p2align 4,,10
	.p2align 3
.L71:
	cmpl	%edx, %eax
	movl	%eax, %ecx
	je	.L65
	movss	(%r8,%rdi,4), %xmm2
	movss	(%r9,%rdi,4), %xmm0
	subss	(%r8,%rax,4), %xmm2
	subss	(%r9,%rax,4), %xmm0
	movss	(%r10,%rdi,4), %xmm1
	mulss	%xmm2, %xmm2
	mulss	%xmm0, %xmm0
	subss	(%r10,%rax,4), %xmm1
	mulss	%xmm1, %xmm1
	addss	%xmm2, %xmm0
	addss	%xmm1, %xmm0
	comiss	%xmm0, %xmm5
	ja	.L73
	comiss	%xmm0, %xmm4
	jbe	.L68
	xorl	%esi, %esi
	cmpl	%edx, %eax
	setg	%sil
	addl	%esi, %r11d
.L68:
	comiss	%xmm0, %xmm3
	jbe	.L65
	cmpl	%edx, %ecx
	jle	.L65
	leal	3(%rcx), %r15d
	testl	%ecx, %ecx
	cmovns	%ecx, %r15d
	andl	$3, %ecx
	sarl	$2, %r15d
	movslq	%r15d, %rsi
	movl	%r12d, %r15d
	movzbl	0(%rbp,%rsi), %esi
	sall	%cl, %r15d
	testl	%esi, %r15d
	je	.L65
	movzbl	(%r14), %ecx
	andl	8(%rsp), %ecx
	cmpl	$1, %ecx
	sbbl	$-1, %r13d
	.p2align 4,,10
	.p2align 3
.L65:
	leaq	1(%rax), %rsi
	cmpl	%edx, %esi
	movq	%rsi, %rax
	movl	%esi, %ecx
	je	.L87
	movss	(%r8,%rdi,4), %xmm11
	movss	(%r9,%rdi,4), %xmm0
	subss	(%r8,%rsi,4), %xmm11
	subss	(%r9,%rsi,4), %xmm0
	movss	(%r10,%rdi,4), %xmm10
	mulss	%xmm11, %xmm11
	mulss	%xmm0, %xmm0
	subss	(%r10,%rsi,4), %xmm10
	mulss	%xmm10, %xmm10
	addss	%xmm11, %xmm0
	addss	%xmm10, %xmm0
	comiss	%xmm0, %xmm5
	ja	.L73
	comiss	%xmm0, %xmm4
	jbe	.L97
	xorl	%r15d, %r15d
	cmpl	%edx, %esi
	setg	%r15b
	addl	%r15d, %r11d
.L97:
	comiss	%xmm0, %xmm3
	jbe	.L87
	cmpl	%edx, %ecx
	jle	.L87
	leal	3(%rcx), %eax
	testl	%ecx, %ecx
	movl	%r12d, %r15d
	cmovns	%ecx, %eax
	andl	$3, %ecx
	sall	%cl, %r15d
	sarl	$2, %eax
	cltq
	movzbl	0(%rbp,%rax), %eax
	testl	%eax, %r15d
	je	.L87
	movzbl	(%r14), %ecx
	andl	8(%rsp), %ecx
	cmpl	$1, %ecx
	sbbl	$-1, %r13d
	.p2align 4,,10
	.p2align 3
.L87:
	leal	1(%rsi), %ecx
	leaq	1(%rsi), %rax
	cmpl	%ebx, %ecx
	jl	.L71
.L72:
	leal	1(%rdi), %eax
	addq	$1, %rdi
	cmpl	%ebx, %eax
	jl	.L64
	movl	%r11d, contacts(%rip)
	movl	%r13d, contacts_loc(%rip)
.L62:
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
.L73:
	.cfi_restore_state
	movq	stderr(%rip), %rdi
	movl	%eax, %ecx
	movl	$.LC5, %esi
	xorl	%eax, %eax
	movl	%r11d, contacts(%rip)
	movl	%r13d, contacts_loc(%rip)
	call	fprintf
	movl	$-2, %edi
	call	exit
	.cfi_endproc
.LFE635:
	.size	count_contacts, .-count_contacts
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC6:
	.string	"r"
.LC7:
	.string	"NULL"
	.section	.rodata.str1.8
	.align 8
.LC8:
	.string	"Warning: file %s not found and not NULL\n"
	.section	.rodata.str1.1
.LC9:
	.string	"%d"
.LC10:
	.string	"File format error %s\n"
	.text
	.p2align 4,,15
	.type	load_laplacian, @function
load_laplacian:
.LFB628:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	movl	$.LC6, %esi
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	movq	%rdi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$24, %rsp
	.cfi_def_cfa_offset 80
	call	fopen
	testq	%rax, %rax
	movq	%rax, %rdi
	movq	%rax, simufiles+40(%rip)
	je	.L131
	movq	lpl_index(%rip), %rdx
	movl	$0, (%rdx)
	movl	N(%rip), %eax
	testl	%eax, %eax
	jle	.L102
	xorl	%ebx, %ebx
	xorl	%ebp, %ebp
	.p2align 4,,10
	.p2align 3
.L112:
	xorl	%eax, %eax
	movq	%rsp, %rdx
	movl	$.LC9, %esi
	call	__isoc99_fscanf
	cmpl	$1, %eax
	jne	.L125
	movl	(%rsp), %r13d
	cmpl	$2, %r13d
	jg	.L127
	testl	%r13d, %r13d
	jle	.L132
	movq	lpl_index(%rip), %r10
	xorl	%eax, %eax
	movl	$.LC9, %esi
	movq	lpl(%rip), %r9
	leal	-1(%r13), %r15d
	movslq	%r13d, %r14
	movq	simufiles+40(%rip), %rdi
	andl	$1, %r15d
	movslq	(%r10,%rbx), %r8
	leaq	(%r9,%r8,4), %rdx
	call	__isoc99_fscanf
	cmpl	$1, %eax
	jne	.L125
	movq	lpl_index(%rip), %rsi
	movl	(%rsp), %eax
	movq	lpl(%rip), %rcx
	movl	N(%rip), %r9d
	leaq	(%rsi,%rbx), %rdi
	movl	(%rdi), %edx
	leal	(%rdx,%rax), %r11d
	subl	%r13d, %r11d
	movslq	%r11d, %r10
	cmpl	%r9d, (%rcx,%r10,4)
	jl	.L115
	subl	$1, %eax
	movl	%eax, (%rsp)
	movl	(%rdi), %edx
.L115:
	subq	$1, %r14
	subl	$1, %r13d
	je	.L106
	testl	%r15d, %r15d
	je	.L109
	movslq	%edx, %rsi
	cltq
	addq	%rsi, %rax
	subq	%r14, %rax
	leaq	(%rcx,%rax,4), %rdx
	jmp	.L130
	.p2align 4,,10
	.p2align 3
.L109:
	movslq	%edx, %r11
	cltq
	movq	simufiles+40(%rip), %rdi
	addq	%r11, %rax
	movl	$.LC9, %esi
	subq	%r14, %rax
	leaq	(%rcx,%rax,4), %rdx
	xorl	%eax, %eax
	call	__isoc99_fscanf
	cmpl	$1, %eax
	jne	.L125
	movq	lpl_index(%rip), %rdi
	movl	(%rsp), %eax
	movq	lpl(%rip), %rsi
	movl	N(%rip), %edx
	addq	%rbx, %rdi
	movl	(%rdi), %ecx
	leal	(%rcx,%rax), %r8d
	subl	%r13d, %r8d
	movslq	%r8d, %r15
	cmpl	%edx, (%rsi,%r15,4)
	jl	.L108
	subl	$1, %eax
	movl	%eax, (%rsp)
	movl	(%rdi), %ecx
.L108:
	cltq
	movslq	%ecx, %rcx
	subq	$1, %r14
	addq	%rcx, %rax
	subl	$1, %r13d
	subq	%r14, %rax
	leaq	(%rsi,%rax,4), %rdx
.L130:
	movq	simufiles+40(%rip), %rdi
	xorl	%eax, %eax
	movl	$.LC9, %esi
	call	__isoc99_fscanf
	cmpl	$1, %eax
	jne	.L125
	movq	lpl_index(%rip), %rsi
	movl	(%rsp), %eax
	movq	lpl(%rip), %rcx
	movl	N(%rip), %r8d
	leaq	(%rsi,%rbx), %rdi
	movl	(%rdi), %edx
	leal	(%rdx,%rax), %r10d
	subl	%r13d, %r10d
	movslq	%r10d, %r15
	cmpl	%r8d, (%rcx,%r15,4)
	jl	.L122
	subl	$1, %eax
	movl	%eax, (%rsp)
	movl	(%rdi), %edx
.L122:
	subq	$1, %r14
	subl	$1, %r13d
	jne	.L109
.L106:
	addq	$4, %rbx
	addl	%edx, %eax
	movl	%eax, (%rsi,%rbx)
	movl	N(%rip), %r13d
	addl	%r13d, %r13d
	cmpl	%r13d, %eax
	jg	.L127
	movslq	(%rdi), %rdi
	leal	1(%rbp), %r14d
	movl	$cmpint, %ecx
	movq	lpl(%rip), %rax
	movl	$4, %edx
	addq	$1, %rbp
	movslq	(%rsp), %rsi
	leaq	(%rax,%rdi,4), %rdi
	call	qsort
	cmpl	%r14d, N(%rip)
	movq	simufiles+40(%rip), %rdi
	jg	.L112
.L102:
	call	fclose
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
.L132:
	.cfi_restore_state
	movq	lpl_index(%rip), %rsi
	movl	%r13d, %eax
	leaq	(%rsi,%rbx), %rdi
	movl	(%rdi), %edx
	jmp	.L106
.L125:
	movq	stderr(%rip), %rdi
	movq	%r12, %rdx
	movl	$.LC10, %esi
	xorl	%eax, %eax
	call	fprintf
	movl	$-3, %edi
	call	exit
.L131:
	movl	$5, %ecx
	movl	$.LC7, %edi
	movq	%r12, %rsi
	repz cmpsb
	je	.L100
	movq	stderr(%rip), %rdi
	movq	%r12, %rdx
	movl	$.LC8, %esi
	xorl	%eax, %eax
	call	fprintf
.L100:
	xorl	%eax, %eax
	call	load_laplacian_null
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
.L127:
	.cfi_restore_state
	movl	$.LC4, %edi
	movl	$67, %edx
	movl	$1, %esi
	movq	stderr(%rip), %rcx
	call	fwrite
	movl	$-4, %edi
	call	exit
	.cfi_endproc
.LFE628:
	.size	load_laplacian, .-load_laplacian
	.p2align 4,,15
	.type	load_localized_stuff, @function
load_localized_stuff:
.LFB634:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	movl	N(%rip), %edi
	testl	%edi, %edi
	je	.L134
	xorl	%eax, %eax
.L135:
	movq	locmask(%rip), %rcx
	movl	%eax, %edx
	shrl	$2, %edx
	movb	$0, (%rcx,%rdx)
	leal	4(%rax), %edx
	cmpl	%edx, N(%rip)
	jbe	.L134
	movq	locmask(%rip), %r10
	shrl	$2, %edx
	movb	$0, (%r10,%rdx)
	leal	8(%rax), %edx
	cmpl	%edx, N(%rip)
	jbe	.L134
	movq	locmask(%rip), %r11
	shrl	$2, %edx
	movb	$0, (%r11,%rdx)
	leal	12(%rax), %edx
	cmpl	%edx, N(%rip)
	jbe	.L134
	movq	locmask(%rip), %rsi
	shrl	$2, %edx
	movb	$0, (%rsi,%rdx)
	leal	16(%rax), %edx
	cmpl	%edx, N(%rip)
	jbe	.L134
	movq	locmask(%rip), %rdi
	shrl	$2, %edx
	movb	$0, (%rdi,%rdx)
	leal	20(%rax), %edx
	cmpl	%edx, N(%rip)
	jbe	.L134
	movq	locmask(%rip), %r8
	shrl	$2, %edx
	movb	$0, (%r8,%rdx)
	leal	24(%rax), %edx
	cmpl	%edx, N(%rip)
	jbe	.L134
	movq	locmask(%rip), %r9
	shrl	$2, %edx
	movb	$0, (%r9,%rdx)
	leal	28(%rax), %edx
	cmpl	%edx, N(%rip)
	jbe	.L134
	movq	locmask(%rip), %rcx
	shrl	$2, %edx
	addl	$32, %eax
	movb	$0, (%rcx,%rdx)
	cmpl	%eax, N(%rip)
	ja	.L135
	.p2align 4,,10
	.p2align 3
.L134:
	movq	%rbx, %rdi
	movl	$.LC6, %esi
	movl	$0, locnum(%rip)
	call	fopen
	testq	%rax, %rax
	movq	%rax, %rdi
	movq	%rax, simufiles+48(%rip)
	jne	.L137
	jmp	.L157
	.p2align 4,,10
	.p2align 3
.L140:
	movl	(%rsp), %edx
	cmpl	%edx, N(%rip)
	jbe	.L139
	leal	3(%rdx), %r9d
	movl	%edx, %ecx
	movq	locmask(%rip), %rsi
	andl	$3, %ecx
	sall	%cl, %eax
	testl	%edx, %edx
	cmovns	%edx, %r9d
	sarl	$2, %r9d
	movslq	%r9d, %r8
	movzbl	(%rsi,%r8), %ebx
	testl	%eax, %ebx
	jne	.L139
	shrl	$2, %edx
	orb	%al, (%rsi,%rdx)
	addl	$1, locnum(%rip)
.L139:
	movq	simufiles+48(%rip), %rdi
.L137:
	xorl	%eax, %eax
	movq	%rsp, %rdx
	movl	$.LC9, %esi
	call	__isoc99_fscanf
	cmpl	$1, %eax
	je	.L140
	movq	simufiles+48(%rip), %rdi
	call	fclose
.L133:
	addq	$16, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
.L157:
	.cfi_restore_state
	movl	$5, %ecx
	movl	$.LC7, %edi
	movq	%rbx, %rsi
	repz cmpsb
	je	.L133
	movq	stderr(%rip), %rdi
	movq	%rbx, %rdx
	movl	$.LC8, %esi
	xorl	%eax, %eax
	call	fprintf
	jmp	.L133
	.cfi_endproc
.LFE634:
	.size	load_localized_stuff, .-load_localized_stuff
	.section	.rodata.str1.1
.LC11:
	.string	"N should be a multiple of 16\n"
	.text
	.p2align 4,,15
	.type	set_param_normalized.isra.1, @function
set_param_normalized.isra.1:
.LFB638:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movd	%xmm1, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movd	%xmm2, %rbx
	subq	$24, %rsp
	.cfi_def_cfa_offset 48
	testb	$15, %dil
	movl	%edi, N(%rip)
	jne	.L160
	cvtsi2sd	%edi, %xmm10
	movsd	.LC12(%rip), %xmm13
	mulsd	.LC13(%rip), %xmm0
	divsd	%xmm10, %xmm13
	mulsd	.LC14(%rip), %xmm10
	divsd	%xmm10, %xmm0
	sqrtsd	%xmm13, %xmm12
	movddup	%xmm12, %xmm11
	cvtpd2ps	%xmm11, %xmm9
	movss	%xmm9, (%rsp)
	movss	%xmm9, lambda(%rip)
	call	cbrt
	movss	(%rsp), %xmm5
	movq	%rbp, beta_uniform(%rip)
	movddup	%xmm0, %xmm8
	cvtpd2ps	%xmm8, %xmm0
	movss	%xmm0, sigma(%rip)
	unpcklps	%xmm0, %xmm0
	cvtps2pd	%xmm0, %xmm7
	unpcklps	%xmm5, %xmm5
	cvtps2pd	%xmm5, %xmm4
	mulss	%xmm0, %xmm0
	movq	%rbx, beta_localized(%rip)
	mulsd	.LC15(%rip), %xmm7
	mulsd	.LC16(%rip), %xmm4
	shufps	$0, %xmm0, %xmm0
	movaps	%xmm0, comp_hc(%rip)
	movddup	%xmm7, %xmm6
	cvtpd2ps	%xmm6, %xmm2
	movss	%xmm2, alfa_uniform(%rip)
	movss	%xmm2, alfa_localized(%rip)
	mulss	%xmm2, %xmm2
	movddup	%xmm4, %xmm3
	cvtpd2ps	%xmm3, %xmm1
	movss	%xmm1, D(%rip)
	shufps	$0, %xmm2, %xmm2
	movaps	%xmm2, comp_sb(%rip)
	movaps	%xmm2, comp_sp(%rip)
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L160:
	.cfi_restore_state
	movl	$.LC11, %edi
	movl	$29, %edx
	movl	$1, %esi
	movq	stderr(%rip), %rcx
	call	fwrite
	orl	$-1, %edi
	call	exit
	.cfi_endproc
.LFE638:
	.size	set_param_normalized.isra.1, .-set_param_normalized.isra.1
	.section	.rodata.str1.1
.LC17:
	.string	"out/"
.LC18:
	.string	"w"
	.text
	.p2align 4,,15
	.type	openfiles, @function
openfiles:
.LFB622:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r12
	movq	%rdi, %r12
	.cfi_offset 12, -24
	pushq	%rbx
	.cfi_offset 3, -32
	call	strlen
	movl	$509, %esi
	movl	$.LC17, %edi
	addl	$16, %eax
	cltq
	addq	$30, %rax
	andq	$-16, %rax
	subq	%rax, %rsp
	call	mkdir
	leaq	15(%rsp), %rbx
	movq	%r12, %rsi
	andq	$-16, %rbx
	movq	%rbx, %rdi
	movl	$796161391, (%rbx)
	movb	$0, 4(%rbx)
	call	strcat
	movq	%rbx, %rax
.L162:
	movl	(%rax), %esi
	leaq	4(%rax), %rcx
	movq	%rcx, %rax
	leal	-16843009(%rsi), %edx
	notl	%esi
	andl	%esi, %edx
	andl	$-2139062144, %edx
	je	.L202
.L233:
	leaq	2(%rax), %rcx
	movl	%edx, %edi
	movl	$.LC18, %esi
	movabsq	$8386094131926628398, %r10
	shrl	$16, %edi
	testl	$32896, %edx
	cmove	%edi, %edx
	movq	%rbx, %rdi
	cmove	%rcx, %rax
	addb	%dl, %dl
	sbbq	$3, %rax
	movq	%r10, (%rax)
	movl	$8021806, 8(%rax)
	call	gzopen
	movq	%r12, %rsi
	movq	%rbx, %rdi
	movq	%rax, simufiles(%rip)
	movl	$796161391, (%rbx)
	movb	$0, 4(%rbx)
	call	strcat
	movq	%rbx, %rax
.L164:
	movl	(%rax), %r8d
	leaq	4(%rax), %rcx
	movq	%rcx, %rax
	leal	-16843009(%r8), %edx
	notl	%r8d
	andl	%r8d, %edx
	andl	$-2139062144, %edx
	jne	.L232
	movl	(%rcx), %esi
	addq	$4, %rax
	leal	-16843009(%rsi), %edx
	notl	%esi
	andl	%esi, %edx
	andl	$-2139062144, %edx
	jne	.L232
	movl	(%rax), %r10d
	leaq	8(%rcx), %rax
	leal	-16843009(%r10), %edx
	notl	%r10d
	andl	%r10d, %edx
	andl	$-2139062144, %edx
	jne	.L232
	movl	8(%rcx), %edi
	leaq	12(%rcx), %rax
	leal	-16843009(%rdi), %edx
	notl	%edi
	andl	%edi, %edx
	andl	$-2139062144, %edx
	jne	.L232
	movl	12(%rcx), %r8d
	leaq	16(%rcx), %rax
	leal	-16843009(%r8), %edx
	notl	%r8d
	andl	%r8d, %edx
	andl	$-2139062144, %edx
	jne	.L232
	movl	16(%rcx), %r9d
	leaq	20(%rcx), %rax
	leal	-16843009(%r9), %edx
	notl	%r9d
	andl	%r9d, %edx
	andl	$-2139062144, %edx
	jne	.L232
	movl	20(%rcx), %r11d
	leaq	24(%rcx), %rax
	leal	-16843009(%r11), %edx
	notl	%r11d
	andl	%r11d, %edx
	andl	$-2139062144, %edx
	jne	.L232
	movl	24(%rcx), %esi
	leaq	28(%rcx), %rax
	leal	-16843009(%rsi), %edx
	notl	%esi
	andl	%esi, %edx
	andl	$-2139062144, %edx
	je	.L164
	.p2align 4,,10
	.p2align 3
.L232:
	leaq	2(%rax), %r11
	movl	%edx, %esi
	movq	%rbx, %rdi
	movabsq	$8386094131539304750, %r9
	shrl	$16, %esi
	testl	$32896, %edx
	cmove	%esi, %edx
	movl	$.LC18, %esi
	cmove	%r11, %rax
	addb	%dl, %dl
	sbbq	$3, %rax
	movq	%r9, (%rax)
	movb	$0, 8(%rax)
	call	fopen
	movq	%r12, %rsi
	movq	%rbx, %rdi
	movq	%rax, simufiles+8(%rip)
	movl	$796161391, (%rbx)
	movb	$0, 4(%rbx)
	call	strcat
	movq	%rbx, %rax
.L166:
	movl	(%rax), %r10d
	leaq	4(%rax), %rcx
	movq	%rcx, %rax
	leal	-16843009(%r10), %edx
	notl	%r10d
	andl	%r10d, %edx
	andl	$-2139062144, %edx
	je	.L204
.L231:
	leaq	2(%rax), %rcx
	movl	%edx, %edi
	movl	$.LC18, %esi
	shrl	$16, %edi
	testl	$32896, %edx
	cmove	%edi, %edx
	movq	%rbx, %rdi
	cmove	%rcx, %rax
	addb	%dl, %dl
	movabsq	$8386094131540419374, %rdx
	sbbq	$3, %rax
	movq	%rdx, (%rax)
	movb	$0, 8(%rax)
	call	fopen
	movq	%r12, %rsi
	movq	%rbx, %rdi
	movq	%rax, simufiles+16(%rip)
	movl	$796161391, (%rbx)
	movb	$0, 4(%rbx)
	call	strcat
	movq	%rbx, %rax
.L168:
	movl	(%rax), %r12d
	leaq	4(%rax), %rcx
	movq	%rcx, %rax
	leal	-16843009(%r12), %edx
	notl	%r12d
	andl	%r12d, %edx
	andl	$-2139062144, %edx
	jne	.L230
	movl	(%rcx), %r11d
	addq	$4, %rax
	leal	-16843009(%r11), %edx
	notl	%r11d
	andl	%r11d, %edx
	andl	$-2139062144, %edx
	jne	.L230
	movl	(%rax), %esi
	leaq	8(%rcx), %rax
	leal	-16843009(%rsi), %edx
	notl	%esi
	andl	%esi, %edx
	andl	$-2139062144, %edx
	jne	.L230
	movl	8(%rcx), %r10d
	leaq	12(%rcx), %rax
	leal	-16843009(%r10), %edx
	notl	%r10d
	andl	%r10d, %edx
	andl	$-2139062144, %edx
	jne	.L230
	movl	12(%rcx), %edi
	leaq	16(%rcx), %rax
	leal	-16843009(%rdi), %edx
	notl	%edi
	andl	%edi, %edx
	andl	$-2139062144, %edx
	jne	.L230
	movl	16(%rcx), %r12d
	leaq	20(%rcx), %rax
	leal	-16843009(%r12), %edx
	notl	%r12d
	andl	%r12d, %edx
	andl	$-2139062144, %edx
	jne	.L230
	movl	20(%rcx), %r8d
	leaq	24(%rcx), %rax
	leal	-16843009(%r8), %edx
	notl	%r8d
	andl	%r8d, %edx
	andl	$-2139062144, %edx
	jne	.L230
	movl	24(%rcx), %r9d
	leaq	28(%rcx), %rax
	leal	-16843009(%r9), %edx
	notl	%r9d
	andl	%r9d, %edx
	andl	$-2139062144, %edx
	je	.L168
	.p2align 4,,10
	.p2align 3
.L230:
	movl	%edx, %r9d
	movq	%rbx, %rdi
	movl	$.LC18, %esi
	leaq	2(%rax), %r8
	shrl	$16, %r9d
	testl	$32896, %edx
	cmove	%r9d, %edx
	cmove	%r8, %rax
	addb	%dl, %dl
	sbbq	$3, %rax
	movl	$1718511918, (%rax)
	movw	$111, 4(%rax)
	call	fopen
	movq	%rax, simufiles+24(%rip)
	leaq	-16(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
	.p2align 4,,10
	.p2align 3
.L202:
	.cfi_restore_state
	movl	(%rcx), %r10d
	addq	$4, %rax
	leal	-16843009(%r10), %edx
	notl	%r10d
	andl	%r10d, %edx
	andl	$-2139062144, %edx
	jne	.L233
	movl	(%rax), %edi
	leaq	8(%rcx), %rax
	leal	-16843009(%rdi), %edx
	notl	%edi
	andl	%edi, %edx
	andl	$-2139062144, %edx
	jne	.L233
	movl	8(%rcx), %r8d
	leaq	12(%rcx), %rax
	leal	-16843009(%r8), %edx
	notl	%r8d
	andl	%r8d, %edx
	andl	$-2139062144, %edx
	jne	.L233
	movl	12(%rcx), %r9d
	leaq	16(%rcx), %rax
	leal	-16843009(%r9), %edx
	notl	%r9d
	andl	%r9d, %edx
	andl	$-2139062144, %edx
	jne	.L233
	movl	16(%rcx), %r11d
	leaq	20(%rcx), %rax
	leal	-16843009(%r11), %edx
	notl	%r11d
	andl	%r11d, %edx
	andl	$-2139062144, %edx
	jne	.L233
	movl	20(%rcx), %esi
	leaq	24(%rcx), %rax
	leal	-16843009(%rsi), %edx
	notl	%esi
	andl	%esi, %edx
	andl	$-2139062144, %edx
	jne	.L233
	movl	24(%rcx), %r10d
	leaq	28(%rcx), %rax
	leal	-16843009(%r10), %edx
	notl	%r10d
	andl	%r10d, %edx
	andl	$-2139062144, %edx
	je	.L162
	jmp	.L233
	.p2align 4,,10
	.p2align 3
.L204:
	movl	(%rcx), %r11d
	addq	$4, %rax
	leal	-16843009(%r11), %edx
	notl	%r11d
	andl	%r11d, %edx
	andl	$-2139062144, %edx
	jne	.L231
	movl	(%rax), %esi
	leaq	8(%rcx), %rax
	leal	-16843009(%rsi), %edx
	notl	%esi
	andl	%esi, %edx
	andl	$-2139062144, %edx
	jne	.L231
	movl	8(%rcx), %r10d
	leaq	12(%rcx), %rax
	leal	-16843009(%r10), %edx
	notl	%r10d
	andl	%r10d, %edx
	andl	$-2139062144, %edx
	jne	.L231
	movl	12(%rcx), %edi
	leaq	16(%rcx), %rax
	leal	-16843009(%rdi), %edx
	notl	%edi
	andl	%edi, %edx
	andl	$-2139062144, %edx
	jne	.L231
	movl	16(%rcx), %r8d
	leaq	20(%rcx), %rax
	leal	-16843009(%r8), %edx
	notl	%r8d
	andl	%r8d, %edx
	andl	$-2139062144, %edx
	jne	.L231
	movl	20(%rcx), %r9d
	leaq	24(%rcx), %rax
	leal	-16843009(%r9), %edx
	notl	%r9d
	andl	%r9d, %edx
	andl	$-2139062144, %edx
	jne	.L231
	movl	24(%rcx), %r11d
	leaq	28(%rcx), %rax
	leal	-16843009(%r11), %edx
	notl	%r11d
	andl	%r11d, %edx
	andl	$-2139062144, %edx
	je	.L166
	jmp	.L231
	.cfi_endproc
.LFE622:
	.size	openfiles, .-openfiles
	.section	.rodata.str1.8
	.align 8
.LC19:
	.string	"# infos about the simulation\nSTARTTIME=\"%s\"\nN=%d\ncfgfile=\"%s\"\nlplfile=\"%s\"\nsigma=%g\nlambda=%g\nD=%g\nalfa_uniform=%g\nalfa_localized=%g\nbeta_uniform=%g\nbeta_localized=%g\nconf_sqradius=%g\nseed=%d\nRELAX_TIME=%llu\nCORRL_TIME=%llu\nSTATISTIC=%d\nDYN_STEPS=%llu\n"
	.text
	.p2align 4,,15
	.type	print_infos.constprop.2, @function
print_infos.constprop.2:
.LFB639:
	.cfi_startproc
	movq	%rbx, -48(%rsp)
	movq	%rbp, -40(%rsp)
	movd	%xmm4, %rbp
	.cfi_offset 6, -48
	.cfi_offset 3, -56
	movq	%r12, -32(%rsp)
	movd	%xmm5, %r12
	.cfi_offset 12, -40
	movq	%r13, -24(%rsp)
	movd	%xmm6, %r13
	.cfi_offset 13, -32
	movq	%r14, -16(%rsp)
	movd	%xmm7, %r14
	.cfi_offset 14, -24
	movq	%r15, -8(%rsp)
	subq	$200, %rsp
	.cfi_def_cfa_offset 208
	.cfi_offset 15, -16
	movd	%xmm3, %r15
	movl	%edi, 108(%rsp)
	xorl	%edi, %edi
	movq	%rdx, 120(%rsp)
	movq	%xmm2, 64(%rsp)
	movl	%ecx, 56(%rsp)
	movsd	%xmm0, 80(%rsp)
	movq	%r8, 48(%rsp)
	movq	%xmm1, 72(%rsp)
	movq	%rsi, 112(%rsp)
	call	time
	leaq	136(%rsp), %rdi
	movq	%rax, 136(%rsp)
	call	ctime
	movl	$10, %esi
	movq	%rax, %rdi
	movq	%rax, %rbx
	call	strchr
	movd	%r14, %xmm7
	movd	%r13, %xmm6
	movb	$0, (%rax)
	movd	%r12, %xmm5
	movd	%rbp, %xmm4
	movq	48(%rsp), %r8
	movd	%r15, %xmm3
	movl	$.LC19, %esi
	movl	56(%rsp), %ecx
	movl	$20, 24(%rsp)
	movl	$8, %eax
	movq	72(%rsp), %r9
	movq	$10, 16(%rsp)
	movq	64(%rsp), %rdx
	movq	$20, 8(%rsp)
	movq	%r8, 32(%rsp)
	movq	simufiles+24(%rip), %rdi
	movl	%ecx, (%rsp)
	movq	112(%rsp), %r8
	movd	%r9, %xmm1
	movl	108(%rsp), %ecx
	movq	120(%rsp), %r9
	movd	%rdx, %xmm2
	movq	%rbx, %rdx
	movsd	80(%rsp), %xmm0
	call	fprintf
	movq	simufiles+24(%rip), %rdi
	call	fflush
	movq	152(%rsp), %rbx
	movq	160(%rsp), %rbp
	movq	168(%rsp), %r12
	movq	176(%rsp), %r13
	movq	184(%rsp), %r14
	movq	192(%rsp), %r15
	addq	$200, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE639:
	.size	print_infos.constprop.2, .-print_infos.constprop.2
	.p2align 4,,15
	.type	set_bead_initial_position, @function
set_bead_initial_position:
.LFB630:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	movslq	%edi, %rbx
	leaq	0(,%rbx,4), %rbp
	subq	$152, %rsp
	.cfi_def_cfa_offset 208
	movss	%xmm0, 128(%rsp)
	movq	dots(%rip), %rax
	movss	%xmm1, 132(%rsp)
	movss	%xmm2, 136(%rsp)
	movss	(%rax,%rbx,4), %xmm0
	call	__isnanf
	xorl	%edx, %edx
	testl	%eax, %eax
	je	.L236
	movl	dsfmt+3072(%rip), %r13d
	leaq	4(,%rbx,4), %r12
	movl	$1001, 140(%rsp)
	movsd	.LC20(%rip), %xmm1
	.p2align 4,,10
	.p2align 3
.L317:
	cmpl	$381, %r13d
	jg	.L326
	leal	1(%r13), %eax
	movslq	%r13d, %rdi
	movq	dsfmt(,%rdi,8), %rsi
	movl	%eax, dsfmt+3072(%rip)
	orq	$1, %rsi
	cmpl	$382, %eax
	movd	%rsi, %xmm4
	subsd	%xmm1, %xmm4
	je	.L327
	cltq
	addl	$2, %r13d
	movq	dsfmt(,%rax,8), %r9
	movl	%r13d, dsfmt+3072(%rip)
	orq	$1, %r9
	cmpl	$382, %r13d
	movd	%r9, %xmm3
	subsd	%xmm1, %xmm3
	jne	.L239
	movl	$dsfmt, %edi
	movsd	%xmm1, (%rsp)
	movl	$1, %r13d
	movsd	%xmm3, 48(%rsp)
	movsd	%xmm4, 16(%rsp)
	call	dsfmt_gen_rand_all
	movsd	16(%rsp), %xmm4
	xorl	%eax, %eax
	movsd	48(%rsp), %xmm3
	movsd	(%rsp), %xmm1
.L241:
	movq	dsfmt(,%rax,8), %r10
	addsd	%xmm4, %xmm4
	movl	%r13d, dsfmt+3072(%rip)
	addsd	%xmm3, %xmm3
	subsd	%xmm1, %xmm4
	orq	$1, %r10
	subsd	%xmm1, %xmm3
	movd	%r10, %xmm0
	addsd	.LC21(%rip), %xmm0
	movapd	%xmm4, %xmm6
	mulsd	%xmm4, %xmm6
	movapd	%xmm3, %xmm2
	mulsd	%xmm3, %xmm2
	addsd	%xmm0, %xmm0
	subsd	%xmm1, %xmm0
	addsd	%xmm6, %xmm2
	movapd	%xmm0, %xmm5
	mulsd	%xmm0, %xmm5
	addsd	%xmm5, %xmm2
	comisd	%xmm1, %xmm2
	jae	.L317
	movsd	.LC22(%rip), %xmm7
	comisd	%xmm2, %xmm7
	ja	.L317
	movss	D(%rip), %xmm2
	movddup	%xmm4, %xmm10
	movddup	%xmm3, %xmm8
	movq	lpl_index(%rip), %r15
	cvtpd2ps	%xmm10, %xmm9
	cvtpd2ps	%xmm8, %xmm4
	mulss	%xmm2, %xmm9
	movddup	%xmm0, %xmm3
	cvtpd2ps	%xmm3, %xmm0
	mulss	%xmm2, %xmm4
	movq	dots+8(%rip), %r14
	mulss	%xmm0, %xmm2
	movq	dots+16(%rip), %r11
	movl	(%r15,%rbp), %edx
	movl	(%r15,%r12), %eax
	addss	128(%rsp), %xmm9
	movq	%r14, 88(%rsp)
	addss	132(%rsp), %xmm4
	movq	%r11, 96(%rsp)
	addss	136(%rsp), %xmm2
	movl	%edx, 120(%rsp)
	cmpl	%edx, %eax
	movss	%xmm9, 72(%rsp)
	movq	dots(%rip), %r15
	movss	%xmm4, 76(%rsp)
	movss	%xmm2, 104(%rsp)
	movl	%eax, 124(%rsp)
	jle	.L245
	movslq	120(%rsp), %r9
	movq	lpl(%rip), %r8
	movl	124(%rsp), %ecx
	movss	lambda(%rip), %xmm12
	salq	$2, %r9
	movq	%r9, 64(%rsp)
	mulss	%xmm12, %xmm12
	addq	%r8, 64(%rsp)
	subl	120(%rsp), %ecx
	movss	%xmm12, 108(%rsp)
	movq	64(%rsp), %rsi
	subl	$1, %ecx
	leaq	4(,%rcx,4), %rdi
	andl	$3, %ecx
	movq	%rdi, 112(%rsp)
	movslq	(%rsi), %r14
	movss	(%r15,%r14,4), %xmm11
	movsd	%xmm1, (%rsp)
	movq	%rcx, 40(%rsp)
	movaps	%xmm11, %xmm0
	movss	%xmm11, 48(%rsp)
	call	__isnanf
	movq	40(%rsp), %rdx
	testl	%eax, %eax
	movsd	(%rsp), %xmm1
	movss	48(%rsp), %xmm2
	jne	.L290
	movq	88(%rsp), %r9
	movss	72(%rsp), %xmm10
	movss	76(%rsp), %xmm8
	movq	96(%rsp), %r8
	subss	%xmm2, %xmm10
	subss	(%r9,%r14,4), %xmm8
	movss	104(%rsp), %xmm9
	mulss	%xmm10, %xmm10
	mulss	%xmm8, %xmm8
	subss	(%r8,%r14,4), %xmm9
	mulss	%xmm9, %xmm9
	addss	%xmm10, %xmm8
	addss	%xmm9, %xmm8
	comiss	108(%rsp), %xmm8
	ja	.L247
.L290:
	movl	$4, %r14d
	cmpq	112(%rsp), %r14
	je	.L245
	testq	%rdx, %rdx
	je	.L248
	cmpq	$1, %rdx
	je	.L315
	cmpq	$2, %rdx
	je	.L316
	movq	64(%rsp), %r10
	movslq	4(%r10), %rdx
	movss	(%r15,%rdx,4), %xmm11
	movq	%rdx, 40(%rsp)
	movsd	%xmm1, (%rsp)
	movaps	%xmm11, %xmm0
	movss	%xmm11, 48(%rsp)
	call	__isnanf
	movq	40(%rsp), %rdx
	testl	%eax, %eax
	movsd	(%rsp), %xmm1
	movss	48(%rsp), %xmm2
	jne	.L292
	movq	88(%rsp), %rax
	movss	72(%rsp), %xmm14
	movss	76(%rsp), %xmm12
	movq	96(%rsp), %r11
	subss	%xmm2, %xmm14
	subss	(%rax,%rdx,4), %xmm12
	movss	104(%rsp), %xmm13
	mulss	%xmm14, %xmm14
	mulss	%xmm12, %xmm12
	subss	(%r11,%rdx,4), %xmm13
	mulss	%xmm13, %xmm13
	addss	%xmm14, %xmm12
	addss	%xmm13, %xmm12
	comiss	108(%rsp), %xmm12
	ja	.L247
.L292:
	addq	$4, %r14
.L316:
	movq	64(%rsp), %rsi
	movslq	(%rsi,%r14), %rcx
	movss	(%r15,%rcx,4), %xmm15
	movsd	%xmm1, (%rsp)
	movq	%rcx, 40(%rsp)
	movaps	%xmm15, %xmm0
	movss	%xmm15, 48(%rsp)
	call	__isnanf
	movq	40(%rsp), %rdx
	testl	%eax, %eax
	movsd	(%rsp), %xmm1
	movss	48(%rsp), %xmm2
	jne	.L295
	movq	88(%rsp), %r8
	movss	72(%rsp), %xmm7
	movss	76(%rsp), %xmm5
	movq	96(%rsp), %rdi
	subss	%xmm2, %xmm7
	subss	(%r8,%rdx,4), %xmm5
	movss	104(%rsp), %xmm6
	mulss	%xmm7, %xmm7
	mulss	%xmm5, %xmm5
	subss	(%rdi,%rdx,4), %xmm6
	mulss	%xmm6, %xmm6
	addss	%xmm7, %xmm5
	addss	%xmm6, %xmm5
	comiss	108(%rsp), %xmm5
	ja	.L247
.L295:
	addq	$4, %r14
.L315:
	movq	64(%rsp), %rdx
	movslq	(%rdx,%r14), %r9
	movss	(%r15,%r9,4), %xmm2
	movsd	%xmm1, (%rsp)
	movq	%r9, 40(%rsp)
	movaps	%xmm2, %xmm0
	movss	%xmm2, 48(%rsp)
	call	__isnanf
	movq	40(%rsp), %rdx
	testl	%eax, %eax
	movsd	(%rsp), %xmm1
	movss	48(%rsp), %xmm2
	jne	.L298
	movq	88(%rsp), %r11
	movss	72(%rsp), %xmm3
	movss	76(%rsp), %xmm0
	movq	96(%rsp), %r10
	subss	%xmm2, %xmm3
	subss	(%r11,%rdx,4), %xmm0
	movss	104(%rsp), %xmm4
	mulss	%xmm3, %xmm3
	mulss	%xmm0, %xmm0
	subss	(%r10,%rdx,4), %xmm4
	mulss	%xmm4, %xmm4
	addss	%xmm3, %xmm0
	addss	%xmm4, %xmm0
	comiss	108(%rsp), %xmm0
	ja	.L247
.L298:
	addq	$4, %r14
	cmpq	112(%rsp), %r14
	je	.L245
.L248:
	movq	64(%rsp), %r11
	movslq	(%r11,%r14), %r10
	movss	(%r15,%r10,4), %xmm13
	movsd	%xmm1, (%rsp)
	movq	%r10, 40(%rsp)
	movaps	%xmm13, %xmm0
	movss	%xmm13, 48(%rsp)
	call	__isnanf
	movq	40(%rsp), %rdx
	testl	%eax, %eax
	movsd	(%rsp), %xmm1
	movss	48(%rsp), %xmm3
	jne	.L246
	movq	88(%rsp), %rcx
	movss	72(%rsp), %xmm6
	movss	76(%rsp), %xmm14
	subss	%xmm3, %xmm6
	movq	96(%rsp), %rax
	subss	(%rcx,%rdx,4), %xmm14
	movss	104(%rsp), %xmm15
	movaps	%xmm6, %xmm5
	mulss	%xmm6, %xmm5
	subss	(%rax,%rdx,4), %xmm15
	mulss	%xmm14, %xmm14
	mulss	%xmm15, %xmm15
	addss	%xmm5, %xmm14
	addss	%xmm15, %xmm14
	comiss	108(%rsp), %xmm14
	ja	.L247
.L246:
	movq	64(%rsp), %rdx
	leaq	4(%r14), %rsi
	movq	%rsi, 80(%rsp)
	movslq	4(%rdx,%r14), %r14
	movss	(%r15,%r14,4), %xmm7
	movsd	%xmm1, (%rsp)
	movaps	%xmm7, %xmm0
	movss	%xmm7, 48(%rsp)
	call	__isnanf
	movsd	(%rsp), %xmm1
	testl	%eax, %eax
	movss	48(%rsp), %xmm3
	jne	.L301
	movq	88(%rsp), %rcx
	movss	72(%rsp), %xmm11
	movss	76(%rsp), %xmm8
	subss	%xmm3, %xmm11
	movq	96(%rsp), %rax
	subss	(%rcx,%r14,4), %xmm8
	movss	104(%rsp), %xmm9
	movaps	%xmm11, %xmm10
	mulss	%xmm11, %xmm10
	subss	(%rax,%r14,4), %xmm9
	mulss	%xmm8, %xmm8
	mulss	%xmm9, %xmm9
	addss	%xmm10, %xmm8
	addss	%xmm9, %xmm8
	comiss	108(%rsp), %xmm8
	ja	.L247
.L301:
	movq	80(%rsp), %rsi
	movq	64(%rsp), %r14
	movslq	4(%r14,%rsi), %r14
	movss	(%r15,%r14,4), %xmm12
	movsd	%xmm1, (%rsp)
	movaps	%xmm12, %xmm0
	movss	%xmm12, 48(%rsp)
	call	__isnanf
	movsd	(%rsp), %xmm1
	testl	%eax, %eax
	movss	48(%rsp), %xmm3
	jne	.L303
	movq	88(%rsp), %r8
	movss	72(%rsp), %xmm5
	movss	76(%rsp), %xmm13
	subss	%xmm3, %xmm5
	movq	96(%rsp), %rdi
	subss	(%r8,%r14,4), %xmm13
	movss	104(%rsp), %xmm14
	movaps	%xmm5, %xmm15
	mulss	%xmm5, %xmm15
	subss	(%rdi,%r14,4), %xmm14
	mulss	%xmm13, %xmm13
	mulss	%xmm14, %xmm14
	addss	%xmm15, %xmm13
	addss	%xmm14, %xmm13
	comiss	108(%rsp), %xmm13
	ja	.L247
.L303:
	movq	64(%rsp), %r9
	movq	80(%rsp), %rdx
	movslq	8(%r9,%rdx), %r14
	movss	(%r15,%r14,4), %xmm6
	movsd	%xmm1, (%rsp)
	movaps	%xmm6, %xmm0
	movss	%xmm6, 48(%rsp)
	call	__isnanf
	movsd	(%rsp), %xmm1
	testl	%eax, %eax
	movss	48(%rsp), %xmm2
	jne	.L305
	movq	88(%rsp), %r11
	movss	72(%rsp), %xmm0
	movss	76(%rsp), %xmm7
	movq	96(%rsp), %r10
	subss	%xmm2, %xmm0
	subss	(%r11,%r14,4), %xmm7
	movss	104(%rsp), %xmm2
	mulss	%xmm0, %xmm0
	mulss	%xmm7, %xmm7
	subss	(%r10,%r14,4), %xmm2
	mulss	%xmm2, %xmm2
	addss	%xmm0, %xmm7
	addss	%xmm2, %xmm7
	comiss	108(%rsp), %xmm7
	ja	.L247
.L305:
	movq	80(%rsp), %r14
	addq	$12, %r14
	cmpq	112(%rsp), %r14
	jne	.L248
.L245:
	movl	N(%rip), %edx
	testl	%edx, %edx
	movl	%edx, 80(%rsp)
	jle	.L249
	movss	sigma(%rip), %xmm2
	xorl	%r14d, %r14d
	subl	$1, %edx
	cvtsi2ss	%r14d, %xmm0
	andl	$3, %edx
	movsd	%xmm1, (%rsp)
	mulss	%xmm2, %xmm2
	movl	%edx, 40(%rsp)
	movss	%xmm2, 64(%rsp)
	call	__isnanf
	movl	40(%rsp), %edx
	testl	%eax, %eax
	movsd	(%rsp), %xmm1
	jne	.L273
	movq	88(%rsp), %r10
	movss	72(%rsp), %xmm13
	movss	76(%rsp), %xmm10
	movq	96(%rsp), %r9
	subss	(%r15), %xmm13
	subss	(%r10), %xmm10
	movss	104(%rsp), %xmm12
	mulss	%xmm13, %xmm13
	movss	64(%rsp), %xmm11
	mulss	%xmm10, %xmm10
	subss	(%r9), %xmm12
	mulss	%xmm12, %xmm12
	addss	%xmm13, %xmm10
	addss	%xmm12, %xmm10
	comiss	%xmm10, %xmm11
	ja	.L247
.L273:
	cmpl	$1, 80(%rsp)
	movl	$1, %r14d
	jle	.L249
	testl	%edx, %edx
	je	.L251
	cmpl	$1, %edx
	je	.L312
	cmpl	$2, %edx
	.p2align 4,,3
	je	.L313
	cvtsi2ss	%r14d, %xmm0
	movsd	%xmm1, (%rsp)
	call	__isnanf
	movsd	(%rsp), %xmm1
	testl	%eax, %eax
	jne	.L275
	movq	88(%rsp), %rax
	movss	72(%rsp), %xmm6
	movss	76(%rsp), %xmm14
	movq	96(%rsp), %r11
	subss	4(%r15), %xmm6
	subss	4(%rax), %xmm14
	movss	104(%rsp), %xmm5
	mulss	%xmm6, %xmm6
	movss	64(%rsp), %xmm15
	mulss	%xmm14, %xmm14
	subss	4(%r11), %xmm5
	mulss	%xmm5, %xmm5
	addss	%xmm6, %xmm14
	addss	%xmm5, %xmm14
	comiss	%xmm14, %xmm15
	ja	.L247
.L275:
	addq	$1, %r14
.L313:
	cvtsi2ss	%r14d, %xmm0
	movsd	%xmm1, (%rsp)
	call	__isnanf
	movsd	(%rsp), %xmm1
	testl	%eax, %eax
	jne	.L278
	movq	88(%rsp), %rdx
	movss	72(%rsp), %xmm3
	movss	76(%rsp), %xmm7
	movq	96(%rsp), %rcx
	subss	(%r15,%r14,4), %xmm3
	subss	(%rdx,%r14,4), %xmm7
	movss	104(%rsp), %xmm0
	mulss	%xmm3, %xmm3
	movss	64(%rsp), %xmm2
	mulss	%xmm7, %xmm7
	subss	(%rcx,%r14,4), %xmm0
	mulss	%xmm0, %xmm0
	addss	%xmm3, %xmm7
	addss	%xmm0, %xmm7
	comiss	%xmm7, %xmm2
	ja	.L247
.L278:
	addq	$1, %r14
.L312:
	cvtsi2ss	%r14d, %xmm0
	movsd	%xmm1, (%rsp)
	call	__isnanf
	movsd	(%rsp), %xmm1
	testl	%eax, %eax
	jne	.L281
	movq	88(%rsp), %rdi
	movss	72(%rsp), %xmm10
	movss	76(%rsp), %xmm4
	movq	96(%rsp), %rsi
	subss	(%r15,%r14,4), %xmm10
	subss	(%rdi,%r14,4), %xmm4
	movss	104(%rsp), %xmm9
	mulss	%xmm10, %xmm10
	movss	64(%rsp), %xmm8
	mulss	%xmm4, %xmm4
	subss	(%rsi,%r14,4), %xmm9
	mulss	%xmm9, %xmm9
	addss	%xmm10, %xmm4
	addss	%xmm9, %xmm4
	comiss	%xmm4, %xmm8
	ja	.L247
.L281:
	addq	$1, %r14
	cmpl	%r14d, 80(%rsp)
	jle	.L249
.L251:
	cvtsi2ss	%r14d, %xmm0
	movsd	%xmm1, (%rsp)
	call	__isnanf
	movsd	(%rsp), %xmm1
	testl	%eax, %eax
	jne	.L250
	movq	88(%rsp), %r8
	movss	72(%rsp), %xmm8
	movss	76(%rsp), %xmm0
	movq	96(%rsp), %rdi
	subss	(%r15,%r14,4), %xmm8
	subss	(%r8,%r14,4), %xmm0
	movss	104(%rsp), %xmm4
	mulss	%xmm8, %xmm8
	movss	64(%rsp), %xmm3
	mulss	%xmm0, %xmm0
	subss	(%rdi,%r14,4), %xmm4
	mulss	%xmm4, %xmm4
	addss	%xmm8, %xmm0
	addss	%xmm4, %xmm0
	comiss	%xmm0, %xmm3
	ja	.L247
.L250:
	addq	$1, %r14
	movsd	%xmm1, (%rsp)
	cvtsi2ss	%r14d, %xmm0
	call	__isnanf
	movsd	(%rsp), %xmm1
	testl	%eax, %eax
	jne	.L284
	movq	88(%rsp), %r9
	movss	72(%rsp), %xmm14
	movss	76(%rsp), %xmm11
	movq	96(%rsp), %r8
	subss	(%r15,%r14,4), %xmm14
	subss	(%r9,%r14,4), %xmm11
	movss	104(%rsp), %xmm13
	mulss	%xmm14, %xmm14
	movss	64(%rsp), %xmm12
	mulss	%xmm11, %xmm11
	subss	(%r8,%r14,4), %xmm13
	mulss	%xmm13, %xmm13
	addss	%xmm14, %xmm11
	addss	%xmm13, %xmm11
	comiss	%xmm11, %xmm12
	ja	.L247
.L284:
	leaq	1(%r14), %r10
	movsd	%xmm1, (%rsp)
	cvtsi2ss	%r10d, %xmm0
	movq	%r10, 40(%rsp)
	call	__isnanf
	movq	40(%rsp), %rdx
	testl	%eax, %eax
	movsd	(%rsp), %xmm1
	jne	.L286
	movq	88(%rsp), %rax
	movss	72(%rsp), %xmm7
	movss	76(%rsp), %xmm15
	movq	96(%rsp), %r11
	subss	(%r15,%rdx,4), %xmm7
	subss	(%rax,%rdx,4), %xmm15
	movss	104(%rsp), %xmm6
	mulss	%xmm7, %xmm7
	movss	64(%rsp), %xmm5
	mulss	%xmm15, %xmm15
	subss	(%r11,%rdx,4), %xmm6
	mulss	%xmm6, %xmm6
	addss	%xmm7, %xmm15
	addss	%xmm6, %xmm15
	comiss	%xmm15, %xmm5
	ja	.L247
.L286:
	leaq	2(%r14), %rcx
	movsd	%xmm1, (%rsp)
	cvtsi2ss	%ecx, %xmm0
	movq	%rcx, 40(%rsp)
	call	__isnanf
	movq	40(%rsp), %rdx
	testl	%eax, %eax
	movsd	(%rsp), %xmm1
	jne	.L288
	movq	88(%rsp), %rdi
	movss	72(%rsp), %xmm3
	movss	76(%rsp), %xmm0
	movq	96(%rsp), %rsi
	subss	(%r15,%rdx,4), %xmm3
	subss	(%rdi,%rdx,4), %xmm0
	movss	104(%rsp), %xmm4
	mulss	%xmm3, %xmm3
	movss	64(%rsp), %xmm2
	mulss	%xmm0, %xmm0
	subss	(%rsi,%rdx,4), %xmm4
	mulss	%xmm4, %xmm4
	addss	%xmm3, %xmm0
	addss	%xmm4, %xmm0
	comiss	%xmm0, %xmm2
	ja	.L247
.L288:
	addq	$3, %r14
	cmpl	%r14d, 80(%rsp)
	jg	.L251
.L249:
	movq	96(%rsp), %rbp
	movq	88(%rsp), %r13
	movss	72(%rsp), %xmm9
	movss	76(%rsp), %xmm1
	movss	104(%rsp), %xmm3
	movss	%xmm9, (%r15,%rbx,4)
	movss	%xmm1, 0(%r13,%rbx,4)
	movq	lpl(%rip), %r12
	movss	%xmm3, 0(%rbp,%rbx,4)
	movl	124(%rsp), %ebp
	movslq	120(%rsp), %rbx
	subl	120(%rsp), %ebp
	leaq	(%r12,%rbx,4), %rbx
	testl	%ebp, %ebp
	jle	.L325
	movl	(%rbx), %edi
	leal	-1(%rbp), %r13d
	movaps	%xmm3, %xmm2
	movss	76(%rsp), %xmm1
	andl	$3, %r13d
	movss	72(%rsp), %xmm0
	call	set_bead_initial_position
	testl	%eax, %eax
	jne	.L256
	cmpl	$1, %ebp
	movl	$1, %r12d
	jle	.L325
	testl	%r13d, %r13d
	je	.L259
	cmpl	$1, %r13d
	je	.L309
	cmpl	$2, %r13d
	je	.L310
	movl	(%rbx,%r12,4), %edi
	movss	104(%rsp), %xmm2
	movss	76(%rsp), %xmm1
	movss	72(%rsp), %xmm0
	call	set_bead_initial_position
	testl	%eax, %eax
	jne	.L256
	movb	$2, %r12b
.L310:
	movl	(%rbx,%r12,4), %edi
	movss	104(%rsp), %xmm2
	movss	76(%rsp), %xmm1
	movss	72(%rsp), %xmm0
	call	set_bead_initial_position
	testl	%eax, %eax
	jne	.L256
	addq	$1, %r12
.L309:
	movl	(%rbx,%r12,4), %edi
	movss	104(%rsp), %xmm2
	movss	76(%rsp), %xmm1
	movss	72(%rsp), %xmm0
	call	set_bead_initial_position
	testl	%eax, %eax
	jne	.L256
	addq	$1, %r12
	cmpl	%r12d, %ebp
	jle	.L325
.L259:
	movl	(%rbx,%r12,4), %edi
	movss	104(%rsp), %xmm2
	movss	76(%rsp), %xmm1
	movss	72(%rsp), %xmm0
	call	set_bead_initial_position
	testl	%eax, %eax
	jne	.L256
	movl	4(%rbx,%r12,4), %edi
	movss	104(%rsp), %xmm2
	movss	76(%rsp), %xmm1
	movss	72(%rsp), %xmm0
	call	set_bead_initial_position
	testl	%eax, %eax
	jne	.L256
	movl	8(%rbx,%r12,4), %edi
	movss	104(%rsp), %xmm2
	movss	76(%rsp), %xmm1
	movss	72(%rsp), %xmm0
	call	set_bead_initial_position
	testl	%eax, %eax
	jne	.L256
	movl	12(%rbx,%r12,4), %edi
	movss	104(%rsp), %xmm2
	movss	76(%rsp), %xmm1
	movss	72(%rsp), %xmm0
	call	set_bead_initial_position
	testl	%eax, %eax
	jne	.L256
	addq	$4, %r12
	cmpl	%r12d, %ebp
	jg	.L259
.L325:
	xorl	%edx, %edx
.L236:
	addq	$152, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	movl	%edx, %eax
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L326:
	.cfi_restore_state
	movl	$dsfmt, %edi
	movsd	%xmm1, (%rsp)
	movl	$2, %r13d
	call	dsfmt_gen_rand_all
	movq	dsfmt(%rip), %rcx
	movq	dsfmt+8(%rip), %rdx
	movsd	(%rsp), %xmm1
	orq	$1, %rcx
	orq	$1, %rdx
	movd	%rcx, %xmm4
	movd	%rdx, %xmm3
	subsd	%xmm1, %xmm4
	subsd	%xmm1, %xmm3
.L239:
	movslq	%r13d, %rax
	addl	$1, %r13d
	jmp	.L241
	.p2align 4,,10
	.p2align 3
.L327:
	movl	$dsfmt, %edi
	movsd	%xmm1, (%rsp)
	movl	$2, %r13d
	movsd	%xmm4, 16(%rsp)
	call	dsfmt_gen_rand_all
	movsd	(%rsp), %xmm1
	movl	$1, %eax
	movq	dsfmt(%rip), %r8
	movsd	16(%rsp), %xmm4
	orq	$1, %r8
	movd	%r8, %xmm3
	subsd	%xmm1, %xmm3
	jmp	.L241
	.p2align 4,,10
	.p2align 3
.L247:
	subl	$1, 140(%rsp)
	jne	.L317
.L256:
	movl	$-1, %edx
	jmp	.L236
	.cfi_endproc
.LFE630:
	.size	set_bead_initial_position, .-set_bead_initial_position
	.section	.rodata.str1.8
	.align 8
.LC25:
	.string	"Error: Topology don't allow RAND beads configuration, please provide a filename\n"
	.text
	.p2align 4,,15
	.type	load_configuration_rand, @function
load_configuration_rand:
.LFB631:
	.cfi_startproc
	movl	N(%rip), %r9d
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	testl	%r9d, %r9d
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	jle	.L328
	movq	dots(%rip), %r8
	movl	%r9d, %ebp
	movq	dots+8(%rip), %r11
	shrl	$2, %ebp
	movq	dots+16(%rip), %r10
	leal	0(,%rbp,4), %r13d
	testl	%r13d, %r13d
	leaq	16(%r8), %rdx
	leaq	16(%r11), %rsi
	leaq	16(%r10), %rcx
	je	.L343
	cmpq	%rdx, %r11
	seta	%al
	cmpq	%rsi, %r8
	seta	%r14b
	orl	%r14d, %eax
	cmpl	$11, %r9d
	seta	%r12b
	andl	%r12d, %eax
	cmpq	%rdx, %r10
	seta	%bl
	cmpq	%rcx, %r8
	seta	%dil
	orl	%edi, %ebx
	andl	%ebx, %eax
	cmpq	%rsi, %r10
	seta	%dl
	cmpq	%rcx, %r11
	seta	%cl
	orl	%ecx, %edx
	testb	%dl, %al
	je	.L343
	cmpl	$4, %ebp
	jbe	.L344
	leal	-5(%rbp), %r14d
	movaps	.LC23(%rip), %xmm0
	movl	$4, %ebx
	leaq	64(%r8), %rdx
	shrl	$2, %r14d
	prefetcht0	368(%r8)
	movups	%xmm0, (%r8)
	leaq	64(%r11), %rcx
	salq	$6, %r14
	movups	%xmm0, (%r11)
	leaq	64(%r10), %rsi
	addq	%rdx, %r14
	movq	%r14, %rax
	movups	%xmm0, (%r10)
	subq	%r8, %rax
	movups	%xmm0, 16(%r8)
	subq	$64, %rax
	movups	%xmm0, 16(%r11)
	shrq	$6, %rax
	movups	%xmm0, 16(%r10)
	andl	$1, %eax
	cmpq	%r14, %rdx
	movups	%xmm0, 32(%r8)
	movups	%xmm0, 32(%r11)
	movups	%xmm0, 32(%r10)
	movups	%xmm0, 48(%r8)
	movups	%xmm0, 48(%r11)
	movups	%xmm0, 48(%r10)
	je	.L331
	testq	%rax, %rax
	je	.L332
	movups	%xmm0, (%rdx)
	prefetcht0	432(%r8)
	movl	$8, %ebx
	movups	%xmm0, (%rcx)
	movups	%xmm0, (%rsi)
	movups	%xmm0, 16(%rdx)
	movups	%xmm0, 16(%rcx)
	movups	%xmm0, 16(%rsi)
	movups	%xmm0, 32(%rdx)
	movups	%xmm0, 32(%rcx)
	movups	%xmm0, 32(%rsi)
	movups	%xmm0, 48(%rdx)
	leaq	128(%r8), %rdx
	cmpq	%r14, %rdx
	movups	%xmm0, 48(%rcx)
	movups	%xmm0, 48(%rsi)
	leaq	128(%r11), %rcx
	leaq	128(%r10), %rsi
	je	.L331
.L332:
	movups	%xmm0, (%rdx)
	prefetcht0	368(%rdx)
	addl	$8, %ebx
	movups	%xmm0, (%rcx)
	prefetcht0	432(%rdx)
	movups	%xmm0, (%rsi)
	movups	%xmm0, 16(%rdx)
	movups	%xmm0, 16(%rcx)
	movups	%xmm0, 16(%rsi)
	movups	%xmm0, 32(%rdx)
	movups	%xmm0, 32(%rcx)
	movups	%xmm0, 32(%rsi)
	movups	%xmm0, 48(%rdx)
	movups	%xmm0, 48(%rcx)
	movups	%xmm0, 48(%rsi)
	movups	%xmm0, 64(%rdx)
	movups	%xmm0, 64(%rcx)
	movups	%xmm0, 64(%rsi)
	movups	%xmm0, 80(%rdx)
	movups	%xmm0, 80(%rcx)
	movups	%xmm0, 80(%rsi)
	movups	%xmm0, 96(%rdx)
	movups	%xmm0, 96(%rcx)
	movups	%xmm0, 96(%rsi)
	movups	%xmm0, 112(%rdx)
	subq	$-128, %rdx
	movups	%xmm0, 112(%rcx)
	subq	$-128, %rcx
	movups	%xmm0, 112(%rsi)
	subq	$-128, %rsi
	cmpq	%r14, %rdx
	jne	.L332
.L331:
	movl	%ebx, %r12d
	movups	%xmm0, (%rdx)
	movl	$16, %eax
	leal	1(%rbx), %edi
	notl	%r12d
	movups	%xmm0, (%rcx)
	addl	%ebp, %r12d
	movups	%xmm0, (%rsi)
	andl	$7, %r12d
	cmpl	%edi, %ebp
	jbe	.L397
	testl	%r12d, %r12d
	je	.L337
	cmpl	$1, %r12d
	je	.L391
	cmpl	$2, %r12d
	je	.L392
	cmpl	$3, %r12d
	je	.L393
	cmpl	$4, %r12d
	je	.L394
	cmpl	$5, %r12d
	je	.L395
	cmpl	$6, %r12d
	je	.L396
	movups	%xmm0, 16(%rdx)
	leal	2(%rbx), %edi
	movl	$32, %eax
	movups	%xmm0, 16(%rcx)
	movups	%xmm0, 16(%rsi)
.L396:
	movups	%xmm0, (%rdx,%rax)
	addl	$1, %edi
	movups	%xmm0, (%rcx,%rax)
	movups	%xmm0, (%rsi,%rax)
	addq	$16, %rax
.L395:
	movups	%xmm0, (%rdx,%rax)
	addl	$1, %edi
	movups	%xmm0, (%rcx,%rax)
	movups	%xmm0, (%rsi,%rax)
	addq	$16, %rax
.L394:
	movups	%xmm0, (%rdx,%rax)
	addl	$1, %edi
	movups	%xmm0, (%rcx,%rax)
	movups	%xmm0, (%rsi,%rax)
	addq	$16, %rax
.L393:
	movups	%xmm0, (%rdx,%rax)
	addl	$1, %edi
	movups	%xmm0, (%rcx,%rax)
	movups	%xmm0, (%rsi,%rax)
	addq	$16, %rax
.L392:
	movups	%xmm0, (%rdx,%rax)
	addl	$1, %edi
	movups	%xmm0, (%rcx,%rax)
	movups	%xmm0, (%rsi,%rax)
	addq	$16, %rax
.L391:
	addl	$1, %edi
	movups	%xmm0, (%rdx,%rax)
	movups	%xmm0, (%rcx,%rax)
	movups	%xmm0, (%rsi,%rax)
	addq	$16, %rax
	cmpl	%edi, %ebp
	jbe	.L397
.L337:
	addl	$8, %edi
	movups	%xmm0, (%rdx,%rax)
	movups	%xmm0, (%rcx,%rax)
	movups	%xmm0, (%rsi,%rax)
	movups	%xmm0, 16(%rdx,%rax)
	movups	%xmm0, 16(%rcx,%rax)
	movups	%xmm0, 16(%rsi,%rax)
	movups	%xmm0, 32(%rdx,%rax)
	movups	%xmm0, 32(%rcx,%rax)
	movups	%xmm0, 32(%rsi,%rax)
	movups	%xmm0, 48(%rdx,%rax)
	movups	%xmm0, 48(%rcx,%rax)
	movups	%xmm0, 48(%rsi,%rax)
	movups	%xmm0, 64(%rdx,%rax)
	movups	%xmm0, 64(%rcx,%rax)
	movups	%xmm0, 64(%rsi,%rax)
	movups	%xmm0, 80(%rdx,%rax)
	movups	%xmm0, 80(%rcx,%rax)
	movups	%xmm0, 80(%rsi,%rax)
	movups	%xmm0, 96(%rdx,%rax)
	movups	%xmm0, 96(%rcx,%rax)
	movups	%xmm0, 96(%rsi,%rax)
	movups	%xmm0, 112(%rdx,%rax)
	movups	%xmm0, 112(%rcx,%rax)
	movups	%xmm0, 112(%rsi,%rax)
	subq	$-128, %rax
	cmpl	%edi, %ebp
	ja	.L337
.L397:
	cmpl	%r13d, %r9d
	movl	%r13d, %esi
	je	.L333
.L330:
	leal	-15(%r9), %ebx
	leal	1(%rsi), %ebp
	cmpl	%ebx, %ebp
	jge	.L334
	cmpl	%esi, %r9d
	jle	.L334
	movslq	%esi, %rax
	salq	$2, %rax
	leaq	(%r8,%rax), %rcx
	leaq	(%r11,%rax), %rdx
	addq	%r10, %rax
	.p2align 4,,10
	.p2align 3
.L335:
	addl	$16, %esi
	prefetcht0	80(%rcx)
	prefetcht0	80(%rdx)
	leal	1(%rsi), %r13d
	movl	$0x7fc00000, (%rcx)
	prefetcht0	80(%rax)
	movl	$0x7fc00000, (%rdx)
	movl	$0x7fc00000, (%rax)
	movl	$0x7fc00000, 4(%rcx)
	movl	$0x7fc00000, 4(%rdx)
	movl	$0x7fc00000, 4(%rax)
	movl	$0x7fc00000, 8(%rcx)
	movl	$0x7fc00000, 8(%rdx)
	movl	$0x7fc00000, 8(%rax)
	movl	$0x7fc00000, 12(%rcx)
	movl	$0x7fc00000, 12(%rdx)
	movl	$0x7fc00000, 12(%rax)
	movl	$0x7fc00000, 16(%rcx)
	movl	$0x7fc00000, 16(%rdx)
	movl	$0x7fc00000, 16(%rax)
	movl	$0x7fc00000, 20(%rcx)
	movl	$0x7fc00000, 20(%rdx)
	movl	$0x7fc00000, 20(%rax)
	movl	$0x7fc00000, 24(%rcx)
	movl	$0x7fc00000, 24(%rdx)
	movl	$0x7fc00000, 24(%rax)
	movl	$0x7fc00000, 28(%rcx)
	movl	$0x7fc00000, 28(%rdx)
	movl	$0x7fc00000, 28(%rax)
	movl	$0x7fc00000, 32(%rcx)
	movl	$0x7fc00000, 32(%rdx)
	movl	$0x7fc00000, 32(%rax)
	movl	$0x7fc00000, 36(%rcx)
	movl	$0x7fc00000, 36(%rdx)
	movl	$0x7fc00000, 36(%rax)
	movl	$0x7fc00000, 40(%rcx)
	movl	$0x7fc00000, 40(%rdx)
	movl	$0x7fc00000, 40(%rax)
	movl	$0x7fc00000, 44(%rcx)
	movl	$0x7fc00000, 44(%rdx)
	movl	$0x7fc00000, 44(%rax)
	movl	$0x7fc00000, 48(%rcx)
	movl	$0x7fc00000, 48(%rdx)
	movl	$0x7fc00000, 48(%rax)
	movl	$0x7fc00000, 52(%rcx)
	movl	$0x7fc00000, 52(%rdx)
	movl	$0x7fc00000, 52(%rax)
	movl	$0x7fc00000, 56(%rcx)
	movl	$0x7fc00000, 56(%rdx)
	movl	$0x7fc00000, 56(%rax)
	movl	$0x7fc00000, 60(%rcx)
	addq	$64, %rcx
	movl	$0x7fc00000, 60(%rdx)
	addq	$64, %rdx
	movl	$0x7fc00000, 60(%rax)
	addq	$64, %rax
	cmpl	%r13d, %ebx
	jg	.L335
.L334:
	movslq	%esi, %rdx
	xorl	%eax, %eax
	salq	$2, %rdx
	leaq	(%r8,%rdx), %rdi
	movl	%esi, %r8d
	leaq	(%r11,%rdx), %rcx
	notl	%r8d
	addq	%r10, %rdx
	leal	1(%rsi), %r10d
	addl	%r9d, %r8d
	movl	$0x7fc00000, (%rdi,%rax,4)
	andl	$3, %r8d
	cmpl	%r10d, %r9d
	movl	$0x7fc00000, (%rcx,%rax,4)
	movl	$0x7fc00000, (%rdx,%rax,4)
	movb	$1, %al
	jle	.L333
	testl	%r8d, %r8d
	je	.L336
	cmpl	$1, %r8d
	je	.L389
	cmpl	$2, %r8d
	je	.L390
	movl	$0x7fc00000, (%rdi,%rax,4)
	movl	$0x7fc00000, (%rcx,%rax,4)
	movl	$0x7fc00000, (%rdx,%rax,4)
	movl	$2, %eax
.L390:
	movl	$0x7fc00000, (%rdi,%rax,4)
	movl	$0x7fc00000, (%rcx,%rax,4)
	movl	$0x7fc00000, (%rdx,%rax,4)
	addq	$1, %rax
.L389:
	movl	$0x7fc00000, (%rdi,%rax,4)
	movl	$0x7fc00000, (%rcx,%rax,4)
	movl	$0x7fc00000, (%rdx,%rax,4)
	addq	$1, %rax
	leal	(%rsi,%rax), %r14d
	cmpl	%r14d, %r9d
	jle	.L333
.L336:
	movl	$0x7fc00000, (%rdi,%rax,4)
	leaq	1(%rax), %r12
	movl	$0x7fc00000, (%rcx,%rax,4)
	leaq	2(%rax), %rbx
	movl	$0x7fc00000, (%rdx,%rax,4)
	leaq	3(%rax), %r11
	addq	$4, %rax
	leal	(%rsi,%rax), %r8d
	movl	$0x7fc00000, (%rdi,%r12,4)
	cmpl	%r8d, %r9d
	movl	$0x7fc00000, (%rcx,%r12,4)
	movl	$0x7fc00000, (%rdx,%r12,4)
	movl	$0x7fc00000, (%rdi,%rbx,4)
	movl	$0x7fc00000, (%rcx,%rbx,4)
	movl	$0x7fc00000, (%rdx,%rbx,4)
	movl	$0x7fc00000, (%rdi,%r11,4)
	movl	$0x7fc00000, (%rcx,%r11,4)
	movl	$0x7fc00000, (%rdx,%r11,4)
	jg	.L336
.L333:
	xorl	%ebx, %ebx
	movl	$1000, %ebp
	.p2align 4,,10
	.p2align 3
.L342:
	movl	%ebx, %edi
	xorps	%xmm1, %xmm1
	movaps	%xmm1, %xmm0
	xorps	%xmm2, %xmm2
	call	set_bead_initial_position
	addl	$1, %ebx
	testl	%eax, %eax
	je	.L340
	testl	%ebp, %ebp
	je	.L398
	subl	$1, %ebp
	xorl	%ebx, %ebx
.L340:
	cmpl	%ebx, N(%rip)
	jg	.L342
.L328:
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
.L344:
	.cfi_restore_state
	movq	%r10, %rsi
	movq	%r11, %rcx
	movq	%r8, %rdx
	xorl	%ebx, %ebx
	movaps	.LC23(%rip), %xmm0
	jmp	.L331
.L398:
	movl	$.LC25, %edi
	movl	$80, %edx
	movl	$1, %esi
	movq	stderr(%rip), %rcx
	call	fwrite
	movl	$-4, %edi
	call	exit
.L343:
	xorl	%esi, %esi
	jmp	.L330
	.cfi_endproc
.LFE631:
	.size	load_configuration_rand, .-load_configuration_rand
	.section	.rodata.str1.1
.LC26:
	.string	"RAND"
	.section	.rodata.str1.8
	.align 8
.LC27:
	.string	"Warning: file %s not found and not NULL or RAND, setted to NULL\n"
	.section	.rodata.str1.1
.LC28:
	.string	"%llu%g%g%g"
	.text
	.p2align 4,,15
	.type	load_configuration, @function
load_configuration:
.LFB633:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	$.LC6, %esi
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$24, %rsp
	.cfi_def_cfa_offset 48
	call	fopen
	testq	%rax, %rax
	movq	%rax, %rdi
	movq	%rax, simufiles+32(%rip)
	je	.L400
	movl	N(%rip), %eax
	xorl	%ebx, %ebx
	testl	%eax, %eax
	je	.L402
	.p2align 4,,10
	.p2align 3
.L409:
	movq	dots(%rip), %rcx
	movl	%ebx, %r8d
	xorl	%eax, %eax
	movq	dots+16(%rip), %r9
	salq	$2, %r8
	movl	$.LC28, %esi
	leaq	8(%rsp), %rdx
	addq	%r8, %rcx
	addq	%r8, %r9
	addq	dots+8(%rip), %r8
	call	__isoc99_fscanf
	cmpl	$4, %eax
	jne	.L410
	addl	$1, %ebx
	movq	simufiles+32(%rip), %rdi
	cmpl	%ebx, N(%rip)
	ja	.L409
.L402:
	call	fclose
.L399:
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L400:
	.cfi_restore_state
	movl	$5, %eax
	movl	$.LC26, %edi
	movq	%rbp, %rsi
	movq	%rax, %rcx
	repz cmpsb
	je	.L411
	movl	$.LC7, %edi
	movq	%rbp, %rsi
	movq	%rax, %rcx
	repz cmpsb
	je	.L405
	movq	stderr(%rip), %rdi
	movq	%rbp, %rdx
	movl	$.LC27, %esi
	xorl	%eax, %eax
	call	fprintf
.L405:
	xorl	%eax, %eax
	call	load_configuration_null
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L411:
	.cfi_restore_state
	xorl	%eax, %eax
	call	load_configuration_rand
	jmp	.L399
.L410:
	movq	stderr(%rip), %rdi
	movq	%rbp, %rdx
	movl	$.LC10, %esi
	xorl	%eax, %eax
	call	fprintf
	movl	$-3, %edi
	call	exit
	.cfi_endproc
.LFE633:
	.size	load_configuration, .-load_configuration
	.section	.rodata.str1.1
.LC29:
	.string	"%llu\t%.10f\t%.10f\t%.10f\n"
	.text
	.p2align 4,,15
	.globl	print_buffer
	.type	print_buffer, @function
print_buffer:
.LFB620:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movl	N(%rip), %eax
	movq	%rdi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movq	mc_time+8(%rip), %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	subq	mc_time(%rip), %rbp
	testl	%eax, %eax
	je	.L412
	xorl	%ebx, %ebx
.L414:
	movq	dots(%rip), %rdi
	movl	%ebx, %ecx
	movl	$3, %eax
	movq	dots+16(%rip), %rsi
	movq	dots+8(%rip), %rdx
	movss	(%rdi,%rcx,4), %xmm0
	movq	%r12, %rdi
	movss	(%rsi,%rcx,4), %xmm2
	movl	$.LC29, %esi
	cvtps2pd	%xmm0, %xmm0
	movss	(%rdx,%rcx,4), %xmm1
	movq	%rbp, %rdx
	cvtps2pd	%xmm2, %xmm2
	cvtps2pd	%xmm1, %xmm1
	call	fprintf
	leal	1(%rbx), %eax
	cmpl	%eax, N(%rip)
	jbe	.L412
	movq	dots(%rip), %r10
	movq	%rbp, %rdx
	movl	$.LC29, %esi
	movq	dots+16(%rip), %r9
	movq	%r12, %rdi
	movq	dots+8(%rip), %r8
	movss	(%r10,%rax,4), %xmm5
	movss	(%r9,%rax,4), %xmm4
	cvtps2pd	%xmm5, %xmm0
	movss	(%r8,%rax,4), %xmm3
	movl	$3, %eax
	cvtps2pd	%xmm4, %xmm2
	cvtps2pd	%xmm3, %xmm1
	call	fprintf
	leal	2(%rbx), %eax
	cmpl	%eax, N(%rip)
	jbe	.L412
	movq	dots(%rip), %rdx
	movl	$.LC29, %esi
	movq	%r12, %rdi
	movq	dots+16(%rip), %rcx
	movq	dots+8(%rip), %r11
	movss	(%rdx,%rax,4), %xmm8
	movq	%rbp, %rdx
	movss	(%rcx,%rax,4), %xmm7
	cvtps2pd	%xmm8, %xmm0
	movss	(%r11,%rax,4), %xmm6
	movl	$3, %eax
	cvtps2pd	%xmm7, %xmm2
	cvtps2pd	%xmm6, %xmm1
	call	fprintf
	leal	3(%rbx), %eax
	cmpl	%eax, N(%rip)
	jbe	.L412
	movq	dots+16(%rip), %rdi
	movq	%rbp, %rdx
	addl	$4, %ebx
	movq	dots+8(%rip), %rsi
	movq	dots(%rip), %r8
	movss	(%rdi,%rax,4), %xmm10
	movq	%r12, %rdi
	movss	(%rsi,%rax,4), %xmm9
	movl	$.LC29, %esi
	cvtps2pd	%xmm10, %xmm2
	movss	(%r8,%rax,4), %xmm11
	movl	$3, %eax
	cvtps2pd	%xmm9, %xmm1
	cvtps2pd	%xmm11, %xmm0
	call	fprintf
	cmpl	%ebx, N(%rip)
	ja	.L414
	.p2align 4,,10
	.p2align 3
.L412:
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE620:
	.size	print_buffer, .-print_buffer
	.section	.rodata.str1.1
.LC30:
	.string	"ENDTIME=\"%s\"\n"
	.text
	.p2align 4,,15
	.globl	closefiles
	.type	closefiles, @function
closefiles:
.LFB623:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	movq	simufiles(%rip), %rdi
	call	gzclose
	movq	simufiles+8(%rip), %rdi
	call	fclose
	movq	simufiles+16(%rip), %rdi
	call	fclose
	xorl	%edi, %edi
	call	time
	leaq	8(%rsp), %rdi
	movq	%rax, 8(%rsp)
	call	ctime
	movl	$10, %esi
	movq	%rax, %rdi
	movq	%rax, %rbx
	call	strchr
	movq	%rbx, %rdx
	movl	$.LC30, %esi
	movb	$0, (%rax)
	xorl	%eax, %eax
	movq	simufiles+24(%rip), %rdi
	call	fprintf
	movq	simufiles+24(%rip), %rdi
	call	fclose
	addq	$16, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE623:
	.size	closefiles, .-closefiles
	.section	.rodata.str1.1
.LC31:
	.string	"Error allocating memory\n"
.LC34:
	.string	"%g\n"
.LC35:
	.string	"%llu\t%g\t%d\t%d\n"
	.text
	.p2align 4,,15
	.globl	simulazione
	.type	simulazione, @function
simulazione:
.LFB636:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	xorl	%esi, %esi
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	movq	%rdi, %r13
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$200, %rsp
	.cfi_def_cfa_offset 256
	movq	8(%rdi), %rcx
	movq	72(%rcx), %rdi
	movq	16(%rcx), %r12
	movq	24(%rcx), %rbx
	movq	32(%rcx), %rbp
	movq	40(%rcx), %r15
	call	strtod
	movq	8(%r13), %r11
	xorl	%esi, %esi
	movq	64(%r11), %rdi
	call	strtod
	movq	8(%r13), %r10
	xorl	%esi, %esi
	movq	56(%r10), %rdi
	movsd	%xmm0, 48(%rsp)
	call	strtod
	movq	8(%r13), %r9
	xorl	%esi, %esi
	movq	48(%r9), %rdi
	movsd	%xmm0, 32(%rsp)
	call	strtod
	movq	8(%r13), %r8
	movl	$10, %edx
	xorl	%esi, %esi
	movq	8(%r8), %rdi
	movsd	%xmm0, 64(%rsp)
	call	strtol
	movsd	48(%rsp), %xmm2
	movsd	32(%rsp), %xmm1
	movl	%eax, %edi
	movsd	64(%rsp), %xmm0
	call	set_param_normalized.isra.1
	stmxcsr	172(%rsp)
	orl	$32832, 172(%rsp)
	ldmxcsr	172(%rsp)
	movq	%r12, %rdi
	call	openfiles
	movl	N(%rip), %eax
	leal	(%rax,%rax), %esi
	leal	(%rsi,%rax), %r14d
	movslq	%esi, %rcx
	movl	$16, %esi
	leal	3(%rax), %edi
	movslq	%r14d, %r14
	salq	$2, %r14
	testl	%eax, %eax
	cmovns	%eax, %edi
	addl	$4, %eax
	cltq
	sarl	$2, %edi
	movslq	%edi, %rdx
	movl	$buffer, %edi
	leaq	(%r14,%rdx), %r13
	leaq	0(%r13,%rax,4), %r12
	leaq	(%r12,%rcx,4), %rdx
	call	posix_memalign
	testl	%eax, %eax
	jne	.L631
	movq	buffer(%rip), %rsi
	movslq	N(%rip), %rdx
	addq	%rsi, %r14
	addq	%rsi, %r13
	movq	%rsi, dots(%rip)
	leaq	(%rsi,%rdx,4), %rdi
	movq	%r14, locmask(%rip)
	leaq	(%rsi,%rdx,8), %rax
	addq	%r12, %rsi
	movq	%rdi, dots+8(%rip)
	movq	%rsi, lpl(%rip)
	leaq	176(%rsp), %rdi
	xorl	%esi, %esi
	movq	%rax, dots+16(%rip)
	movq	%r13, lpl_index(%rip)
	call	gettimeofday
	movl	$19937, %edx
	movl	$dsfmt, %edi
	movq	184(%rsp), %r14
	imull	176(%rsp), %r14d
	movl	%r14d, %esi
	call	dsfmt_chk_init_gen_rand
	movq	%rbp, %rdi
	call	load_laplacian
	movq	%rbx, %rdi
	call	load_configuration
	movq	%r15, %rdi
	call	load_localized_stuff
	xorl	%eax, %eax
	call	count_contacts
	movl	N(%rip), %edi
	movl	%r14d, %ecx
	movsd	.LC32(%rip), %xmm7
	movss	sigma(%rip), %xmm0
	movss	alfa_localized(%rip), %xmm4
	movsd	beta_localized(%rip), %xmm6
	movq	%rbp, %rdx
	movq	%rbx, %rsi
	movsd	beta_uniform(%rip), %xmm5
	cvtps2pd	%xmm0, %xmm0
	cvtps2pd	%xmm4, %xmm4
	movss	alfa_uniform(%rip), %xmm3
	movl	$221, %r8d
	movss	D(%rip), %xmm9
	movss	lambda(%rip), %xmm8
	cvtps2pd	%xmm3, %xmm3
	cvtps2pd	%xmm9, %xmm2
	movq	$221, mc_time+8(%rip)
	cvtps2pd	%xmm8, %xmm1
	call	print_infos.constprop.2
	movq	mc_time+8(%rip), %rax
	testq	%rax, %rax
	movq	%rax, mc_time(%rip)
	je	.L425
	leaq	-30(%rax), %rbx
	movsd	.LC20(%rip), %xmm3
	movl	$0, 124(%rsp)
	movq	%rbx, 144(%rsp)
	movl	$0, 168(%rsp)
	.p2align 4,,10
	.p2align 3
.L474:
	cmpq	%rax, 144(%rsp)
	je	.L426
	addl	$1, 124(%rsp)
.L427:
	movl	dsfmt+3072(%rip), %ebx
	cmpl	$381, %ebx
	jg	.L453
	movslq	%ebx, %rax
	addl	$1, %ebx
.L454:
	movl	dsfmt(,%rax,8), %eax
	xorl	%edx, %edx
	movl	%ebx, dsfmt+3072(%rip)
	movq	dots+16(%rip), %r10
	movq	dots+8(%rip), %r9
	movq	dots(%rip), %r8
	divl	N(%rip)
	movl	%edx, %r12d
	movl	%edx, %ebp
	movss	(%r10,%r12,4), %xmm14
	leaq	0(,%r12,4), %r15
	movss	(%r9,%r12,4), %xmm13
	movq	%r15, 112(%rsp)
	movss	(%r8,%r12,4), %xmm12
	movss	%xmm14, 120(%rsp)
	movss	%xmm13, 96(%rsp)
	movss	%xmm12, 104(%rsp)
	.p2align 4,,10
	.p2align 3
.L611:
	cmpl	$381, %ebx
	jg	.L632
	leal	1(%rbx), %eax
	movslq	%ebx, %rcx
	movq	dsfmt(,%rcx,8), %r11
	movl	%eax, dsfmt+3072(%rip)
	orq	$1, %r11
	cmpl	$382, %eax
	movd	%r11, %xmm1
	subsd	%xmm3, %xmm1
	je	.L633
	cltq
	addl	$2, %ebx
	movq	dsfmt(,%rax,8), %rdi
	movl	%ebx, dsfmt+3072(%rip)
	orq	$1, %rdi
	cmpl	$382, %ebx
	movd	%rdi, %xmm2
	subsd	%xmm3, %xmm2
	jne	.L457
	movl	$dsfmt, %edi
	movsd	%xmm1, 32(%rsp)
	movl	$1, %ebx
	movsd	%xmm2, 48(%rsp)
	movsd	%xmm3, 16(%rsp)
	call	dsfmt_gen_rand_all
	movsd	16(%rsp), %xmm3
	xorl	%eax, %eax
	movsd	48(%rsp), %xmm2
	movsd	32(%rsp), %xmm1
.L459:
	movq	dsfmt(,%rax,8), %r8
	addsd	%xmm1, %xmm1
	movl	%ebx, dsfmt+3072(%rip)
	addsd	%xmm2, %xmm2
	subsd	%xmm3, %xmm1
	orq	$1, %r8
	subsd	%xmm3, %xmm2
	movd	%r8, %xmm4
	addsd	.LC21(%rip), %xmm4
	movapd	%xmm1, %xmm8
	mulsd	%xmm1, %xmm8
	movapd	%xmm2, %xmm0
	mulsd	%xmm2, %xmm0
	addsd	%xmm4, %xmm4
	subsd	%xmm3, %xmm4
	addsd	%xmm8, %xmm0
	movapd	%xmm4, %xmm15
	mulsd	%xmm4, %xmm15
	addsd	%xmm15, %xmm0
	comisd	%xmm3, %xmm0
	jae	.L611
	movsd	.LC22(%rip), %xmm9
	comisd	%xmm0, %xmm9
	ja	.L611
	movq	lpl_index(%rip), %r9
	movddup	%xmm1, %xmm1
	movddup	%xmm2, %xmm2
	movss	D(%rip), %xmm7
	cvtpd2ps	%xmm1, %xmm1
	cvtpd2ps	%xmm2, %xmm2
	movddup	%xmm4, %xmm0
	cvtpd2ps	%xmm0, %xmm4
	mulss	%xmm7, %xmm1
	movl	(%r9,%r12,4), %ecx
	leal	1(%rbp), %r12d
	movl	(%r9,%r12,4), %edx
	mulss	%xmm7, %xmm2
	mulss	%xmm7, %xmm4
	addss	104(%rsp), %xmm1
	cmpl	%edx, %ecx
	addss	96(%rsp), %xmm2
	addss	120(%rsp), %xmm4
	jge	.L462
	movq	lpl(%rip), %r13
	movslq	%ecx, %r15
	subl	%ecx, %edx
	movq	dots(%rip), %r8
	movaps	%xmm1, %xmm11
	movaps	%xmm2, %xmm10
	movq	dots+8(%rip), %rax
	movaps	%xmm4, %xmm6
	subl	$1, %edx
	movq	dots+16(%rip), %r9
	leaq	4(,%rdx,4), %rdi
	movq	%rdx, %rsi
	leaq	0(%r13,%r15,4), %rcx
	movss	lambda(%rip), %xmm5
	andl	$3, %esi
	movslq	(%rcx), %r10
	mulss	%xmm5, %xmm5
	subss	(%r8,%r10,4), %xmm11
	subss	(%rax,%r10,4), %xmm10
	subss	(%r9,%r10,4), %xmm6
	mulss	%xmm11, %xmm11
	mulss	%xmm10, %xmm10
	mulss	%xmm6, %xmm6
	addss	%xmm11, %xmm10
	addss	%xmm6, %xmm10
	comiss	%xmm5, %xmm10
	ja	.L473
	movl	$4, %edx
	cmpq	%rdi, %rdx
	je	.L465
	testq	%rsi, %rsi
	je	.L464
	cmpq	$1, %rsi
	je	.L591
	cmpq	$2, %rsi
	je	.L592
	movslq	4(%rcx), %r13
	movaps	%xmm1, %xmm6
	movaps	%xmm2, %xmm8
	movaps	%xmm4, %xmm9
	subss	(%r8,%r13,4), %xmm6
	subss	(%rax,%r13,4), %xmm8
	subss	(%r9,%r13,4), %xmm9
	mulss	%xmm6, %xmm6
	mulss	%xmm8, %xmm8
	mulss	%xmm9, %xmm9
	addss	%xmm6, %xmm8
	addss	%xmm9, %xmm8
	comiss	%xmm5, %xmm8
	ja	.L473
	movl	$8, %edx
.L592:
	movslq	(%rcx,%rdx), %r11
	movaps	%xmm1, %xmm7
	movaps	%xmm2, %xmm10
	movaps	%xmm4, %xmm11
	subss	(%r8,%r11,4), %xmm7
	subss	(%rax,%r11,4), %xmm10
	subss	(%r9,%r11,4), %xmm11
	mulss	%xmm7, %xmm7
	mulss	%xmm10, %xmm10
	mulss	%xmm11, %xmm11
	addss	%xmm7, %xmm10
	addss	%xmm11, %xmm10
	comiss	%xmm5, %xmm10
	ja	.L473
	addq	$4, %rdx
.L591:
	movslq	(%rcx,%rdx), %r15
	movaps	%xmm1, %xmm13
	movaps	%xmm2, %xmm0
	movaps	%xmm4, %xmm14
	subss	(%r8,%r15,4), %xmm13
	subss	(%rax,%r15,4), %xmm0
	subss	(%r9,%r15,4), %xmm14
	mulss	%xmm13, %xmm13
	mulss	%xmm0, %xmm0
	mulss	%xmm14, %xmm14
	addss	%xmm13, %xmm0
	addss	%xmm14, %xmm0
	comiss	%xmm5, %xmm0
	ja	.L473
	addq	$4, %rdx
	cmpq	%rdi, %rdx
	je	.L465
.L464:
	movslq	(%rcx,%rdx), %r14
	movaps	%xmm1, %xmm14
	movaps	%xmm2, %xmm12
	movaps	%xmm4, %xmm13
	subss	(%r8,%r14,4), %xmm14
	subss	(%rax,%r14,4), %xmm12
	subss	(%r9,%r14,4), %xmm13
	mulss	%xmm14, %xmm14
	mulss	%xmm12, %xmm12
	mulss	%xmm13, %xmm13
	addss	%xmm14, %xmm12
	addss	%xmm13, %xmm12
	comiss	%xmm5, %xmm12
	ja	.L473
	movslq	4(%rcx,%rdx), %r11
	movaps	%xmm1, %xmm9
	movaps	%xmm2, %xmm15
	movaps	%xmm4, %xmm8
	subss	(%r8,%r11,4), %xmm9
	subss	(%rax,%r11,4), %xmm15
	subss	(%r9,%r11,4), %xmm8
	mulss	%xmm9, %xmm9
	mulss	%xmm15, %xmm15
	mulss	%xmm8, %xmm8
	addss	%xmm9, %xmm15
	addss	%xmm8, %xmm15
	comiss	%xmm5, %xmm15
	ja	.L473
	movslq	8(%rcx,%rdx), %rsi
	movaps	%xmm1, %xmm8
	movaps	%xmm2, %xmm12
	movaps	%xmm4, %xmm15
	subss	(%r8,%rsi,4), %xmm8
	subss	(%rax,%rsi,4), %xmm12
	subss	(%r9,%rsi,4), %xmm15
	mulss	%xmm8, %xmm8
	mulss	%xmm12, %xmm12
	mulss	%xmm15, %xmm15
	addss	%xmm8, %xmm12
	addss	%xmm15, %xmm12
	comiss	%xmm5, %xmm12
	ja	.L473
	movslq	12(%rcx,%rdx), %r12
	movaps	%xmm1, %xmm10
	movaps	%xmm2, %xmm9
	movaps	%xmm4, %xmm6
	subss	(%r8,%r12,4), %xmm10
	subss	(%rax,%r12,4), %xmm9
	subss	(%r9,%r12,4), %xmm6
	mulss	%xmm10, %xmm10
	mulss	%xmm9, %xmm9
	mulss	%xmm6, %xmm6
	addss	%xmm10, %xmm9
	addss	%xmm6, %xmm9
	comiss	%xmm5, %xmm9
	ja	.L473
	addq	$16, %rdx
	cmpq	%rdi, %rdx
	jne	.L464
.L465:
	movl	$1, %edx
	movl	%ebp, %ecx
	movl	%ebp, %edi
	andl	$3, %ecx
	movl	%edx, %r10d
	andl	$-4, %edi
	sall	%cl, %r10d
	movq	locmask(%rip), %r15
	movl	%edi, 120(%rsp)
	movl	%r10d, %r14d
	movq	112(%rsp), %r11
	movaps	%xmm1, %xmm15
	xorl	$15, %r14d
	movq	112(%rsp), %r13
	testl	%ebp, %ebp
	leal	3(%rbp), %r12d
	movq	112(%rsp), %r10
	movl	%ebp, %ecx
	cmovns	%ebp, %r12d
	movl	N(%rip), %edi
	andl	$3, %ecx
	addq	%r8, %r11
	sall	%cl, %edx
	shufps	$0, %xmm15, %xmm15
	sarl	$2, %r12d
	movss	(%r11), %xmm14
	addq	%rax, %r13
	movslq	%r12d, %rsi
	addq	%r9, %r10
	movaps	%xmm15, %xmm11
	movzbl	(%r15,%rsi), %esi
	movaps	%xmm14, %xmm0
	movaps	%xmm2, %xmm12
	shufps	$0, %xmm0, %xmm0
	movss	(%r10), %xmm15
	movaps	%xmm0, %xmm7
	movss	0(%r13), %xmm0
	movaps	%xmm4, %xmm13
	movq	%r15, 96(%rsp)
	shufps	$0, %xmm12, %xmm12
	shufps	$0, %xmm13, %xmm13
	movq	%r11, 104(%rsp)
	andl	%edx, %esi
	testl	%edi, %edi
	movq	%r13, 128(%rsp)
	shufps	$0, %xmm0, %xmm0
	movq	%r10, 136(%rsp)
	shufps	$0, %xmm15, %xmm15
	je	.L480
	leaq	64(%rax), %rcx
	movl	%ebx, 164(%rsp)
	addq	$64, %r8
	movaps	comp_hc(%rip), %xmm14
	xorl	%r13d, %r13d
	xorl	%r12d, %r12d
	movaps	comp_sb(%rip), %xmm10
	xorl	%r11d, %r11d
	xorl	%r10d, %r10d
	movaps	comp_sp(%rip), %xmm9
	xorl	%edx, %edx
	xorl	%eax, %eax
	movl	$15, %ebp
	movl	%edi, 12(%rsp)
	movaps	%xmm0, %xmm8
	movss	%xmm1, 152(%rsp)
	movl	%r14d, %ebx
	movss	%xmm2, 156(%rsp)
	movss	%xmm4, 160(%rsp)
	jmp	.L468
	.p2align 4,,10
	.p2align 3
.L635:
	subps	%xmm7, %xmm6
	subps	%xmm8, %xmm5
	subps	%xmm15, %xmm4
	mulps	%xmm6, %xmm6
	mulps	%xmm5, %xmm5
	mulps	%xmm4, %xmm4
	addps	%xmm5, %xmm6
	addps	%xmm6, %xmm4
	movaps	%xmm0, %xmm6
	cmpltps	%xmm10, %xmm6
	movaps	%xmm4, %xmm5
	cmpltps	%xmm10, %xmm5
	movmskps	%xmm6, %edi
	andl	%edx, %edi
	movzbl	%dil, %r14d
	movmskps	%xmm5, %edi
	popcntl	%r14d, %r15d
	addl	%r15d, %r11d
	andl	%edx, %edi
	movzbl	%dil, %r14d
	popcntl	%r14d, %r15d
	addl	%r15d, %r10d
	testl	%esi, %esi
	je	.L467
	movl	%eax, %edi
	cmpltps	%xmm9, %xmm4
	sarl	$2, %edi
	cmpltps	%xmm9, %xmm0
	movslq	%edi, %r15
	movq	96(%rsp), %rdi
	movmskps	%xmm4, %r14d
	andb	(%rdi,%r15), %dl
	andl	%edx, %r14d
	movzbl	%r14b, %r15d
	movmskps	%xmm0, %r14d
	andl	%r14d, %edx
	popcntl	%r15d, %edi
	addl	%edi, %r12d
	movzbl	%dl, %r15d
	popcntl	%r15d, %edx
	addl	%edx, %r13d
.L467:
	addl	$4, %eax
	addq	$16, %r8
	addq	$16, %rcx
	addq	$16, %r9
	cmpl	%eax, 12(%rsp)
	movl	%eax, %edx
	jbe	.L634
.L468:
	movaps	-64(%r8), %xmm6
	prefetcht0	(%r8)
	prefetcht0	(%rcx)
	movaps	-64(%rcx), %xmm5
	movaps	%xmm6, %xmm1
	movaps	(%r9), %xmm4
	movaps	%xmm5, %xmm2
	subps	%xmm11, %xmm1
	cmpl	%edx, 120(%rsp)
	subps	%xmm12, %xmm2
	movaps	%xmm4, %xmm0
	subps	%xmm13, %xmm0
	movl	%ebp, %edx
	mulps	%xmm1, %xmm1
	mulps	%xmm2, %xmm2
	mulps	%xmm0, %xmm0
	cmove	%ebx, %edx
	addps	%xmm2, %xmm1
	addps	%xmm1, %xmm0
	movaps	%xmm0, %xmm2
	cmpltps	%xmm14, %xmm2
	movmskps	%xmm2, %edi
	testb	%dil, %dl
	je	.L635
	.p2align 4,,10
	.p2align 3
.L473:
	movq	mc_time(%rip), %rax
	subq	$1, %rax
	testq	%rax, %rax
	movq	%rax, mc_time(%rip)
	jne	.L474
.L425:
	xorl	%eax, %eax
	call	closefiles
	addq	$200, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L632:
	.cfi_restore_state
	movl	$dsfmt, %edi
	movsd	%xmm3, 16(%rsp)
	movl	$2, %ebx
	call	dsfmt_gen_rand_all
	movq	dsfmt(%rip), %r14
	movq	dsfmt+8(%rip), %r13
	movsd	16(%rsp), %xmm3
	orq	$1, %r14
	orq	$1, %r13
	movd	%r14, %xmm1
	movd	%r13, %xmm2
	subsd	%xmm3, %xmm1
	subsd	%xmm3, %xmm2
.L457:
	movslq	%ebx, %rax
	addl	$1, %ebx
	jmp	.L459
	.p2align 4,,10
	.p2align 3
.L633:
	movl	$dsfmt, %edi
	movsd	%xmm1, 32(%rsp)
	movl	$2, %ebx
	movsd	%xmm3, 16(%rsp)
	call	dsfmt_gen_rand_all
	movsd	16(%rsp), %xmm3
	movl	$1, %eax
	movq	dsfmt(%rip), %rsi
	movsd	32(%rsp), %xmm1
	orq	$1, %rsi
	movd	%rsi, %xmm2
	subsd	%xmm3, %xmm2
	jmp	.L459
.L453:
	movl	$dsfmt, %edi
	movsd	%xmm3, 16(%rsp)
	movl	$1, %ebx
	call	dsfmt_gen_rand_all
	xorl	%eax, %eax
	movsd	16(%rsp), %xmm3
	jmp	.L454
.L634:
	movl	%r11d, %ebp
	subl	%r12d, %r13d
	xorpd	%xmm7, %xmm7
	cvtsi2sd	%r13d, %xmm0
	subl	%r10d, %ebp
	movl	164(%rsp), %ebx
	cvtsi2sd	%ebp, %xmm10
	movss	152(%rsp), %xmm1
	movss	156(%rsp), %xmm2
	movss	160(%rsp), %xmm4
	mulsd	beta_localized(%rip), %xmm0
	mulsd	beta_uniform(%rip), %xmm10
	addsd	%xmm10, %xmm0
	comisd	%xmm0, %xmm7
	ja	.L636
.L469:
	addl	%ebp, contacts(%rip)
	movq	104(%rsp), %r12
	movq	128(%rsp), %r8
	movq	136(%rsp), %rbp
	addl	%r13d, contacts_loc(%rip)
	addl	$1, 168(%rsp)
	movss	%xmm1, (%r12)
	movss	%xmm2, (%r8)
	movss	%xmm4, 0(%rbp)
	jmp	.L473
.L636:
	movss	%xmm1, 32(%rsp)
	movss	%xmm2, 48(%rsp)
	movsd	%xmm3, 16(%rsp)
	movss	%xmm4, 80(%rsp)
	call	__exp_finite
	cmpl	$382, %ebx
	movss	32(%rsp), %xmm1
	movss	48(%rsp), %xmm2
	movsd	16(%rsp), %xmm3
	movss	80(%rsp), %xmm4
	je	.L471
	movslq	%ebx, %rax
	addl	$1, %ebx
.L472:
	movq	dsfmt(,%rax,8), %rax
	movl	%ebx, dsfmt+3072(%rip)
	orq	$1, %rax
	movd	%rax, %xmm11
	subsd	%xmm3, %xmm11
	comisd	%xmm0, %xmm11
	jae	.L473
	movq	112(%rsp), %rsi
	movq	112(%rsp), %rcx
	movq	112(%rsp), %rbx
	addq	dots(%rip), %rsi
	addq	dots+8(%rip), %rcx
	addq	dots+16(%rip), %rbx
	movq	%rsi, 104(%rsp)
	movq	%rcx, 128(%rsp)
	movq	%rbx, 136(%rsp)
	jmp	.L469
.L471:
	movl	$dsfmt, %edi
	movsd	%xmm0, 64(%rsp)
	movl	$1, %ebx
	movss	%xmm1, 32(%rsp)
	movss	%xmm2, 48(%rsp)
	movsd	%xmm3, 16(%rsp)
	movss	%xmm4, 80(%rsp)
	call	dsfmt_gen_rand_all
	xorl	%eax, %eax
	movss	80(%rsp), %xmm4
	movsd	16(%rsp), %xmm3
	movss	48(%rsp), %xmm2
	movss	32(%rsp), %xmm1
	movsd	64(%rsp), %xmm0
	jmp	.L472
.L462:
	movq	dots(%rip), %r8
	movq	dots+8(%rip), %rax
	movq	dots+16(%rip), %r9
	jmp	.L465
.L480:
	xorl	%r13d, %r13d
	xorl	%ebp, %ebp
	jmp	.L469
.L426:
	movl	N(%rip), %r11d
	subq	$10, 144(%rsp)
	testl	%r11d, %r11d
	jle	.L428
	movq	dots(%rip), %rcx
	movl	%r11d, %r10d
	movq	dots+8(%rip), %rsi
	movq	dots+16(%rip), %rdi
	movq	%rcx, %rbp
	andl	$15, %ebp
	shrq	$2, %rbp
	negq	%rbp
	andl	$3, %ebp
	cmpl	%r11d, %ebp
	cmovbe	%ebp, %r10d
	testl	%r10d, %r10d
	movl	%r10d, %r9d
	je	.L475
	leal	-1(%r9), %r8d
	movss	(%rcx), %xmm0
	movl	$1, %ebp
	andl	$7, %r8d
	cmpl	$1, %r10d
	movss	(%rsi), %xmm1
	movss	(%rdi), %xmm2
	movl	$1, %edx
	jbe	.L612
	testl	%r8d, %r8d
	je	.L430
	cmpl	$1, %r8d
	je	.L605
	cmpl	$2, %r8d
	je	.L606
	cmpl	$3, %r8d
	je	.L607
	cmpl	$4, %r8d
	je	.L608
	cmpl	$5, %r8d
	je	.L609
	cmpl	$6, %r8d
	je	.L610
	addss	4(%rcx), %xmm0
	movl	$2, %edx
	addss	4(%rsi), %xmm1
	addss	4(%rdi), %xmm2
.L610:
	addss	(%rcx,%rdx,4), %xmm0
	addss	(%rsi,%rdx,4), %xmm1
	addss	(%rdi,%rdx,4), %xmm2
	addq	$1, %rdx
.L609:
	addss	(%rcx,%rdx,4), %xmm0
	addss	(%rsi,%rdx,4), %xmm1
	addss	(%rdi,%rdx,4), %xmm2
	addq	$1, %rdx
.L608:
	addss	(%rcx,%rdx,4), %xmm0
	addss	(%rsi,%rdx,4), %xmm1
	addss	(%rdi,%rdx,4), %xmm2
	addq	$1, %rdx
.L607:
	addss	(%rcx,%rdx,4), %xmm0
	addss	(%rsi,%rdx,4), %xmm1
	addss	(%rdi,%rdx,4), %xmm2
	addq	$1, %rdx
.L606:
	addss	(%rcx,%rdx,4), %xmm0
	addss	(%rsi,%rdx,4), %xmm1
	addss	(%rdi,%rdx,4), %xmm2
	addq	$1, %rdx
.L605:
	addss	(%rcx,%rdx,4), %xmm0
	leal	1(%rdx), %ebp
	addss	(%rsi,%rdx,4), %xmm1
	addss	(%rdi,%rdx,4), %xmm2
	addq	$1, %rdx
	cmpl	%edx, %r10d
	jbe	.L612
.L430:
	addss	(%rcx,%rdx,4), %xmm0
	leaq	1(%rdx), %rbx
	addss	(%rsi,%rdx,4), %xmm1
	leaq	2(%rdx), %r14
	addss	(%rdi,%rdx,4), %xmm2
	leaq	3(%rdx), %r13
	addss	(%rcx,%rbx,4), %xmm0
	leaq	4(%rdx), %r8
	addss	(%rsi,%rbx,4), %xmm1
	leaq	5(%rdx), %r12
	addss	(%rdi,%rbx,4), %xmm2
	leaq	6(%rdx), %r15
	addss	(%rcx,%r14,4), %xmm0
	leaq	7(%rdx), %rbp
	addq	$8, %rdx
	addss	(%rsi,%r14,4), %xmm1
	addss	(%rdi,%r14,4), %xmm2
	addss	(%rcx,%r13,4), %xmm0
	addss	(%rsi,%r13,4), %xmm1
	addss	(%rdi,%r13,4), %xmm2
	addss	(%rcx,%r8,4), %xmm0
	addss	(%rsi,%r8,4), %xmm1
	addss	(%rdi,%r8,4), %xmm2
	addss	(%rcx,%r12,4), %xmm0
	addss	(%rsi,%r12,4), %xmm1
	addss	(%rdi,%r12,4), %xmm2
	addss	(%rcx,%r15,4), %xmm0
	addss	(%rsi,%r15,4), %xmm1
	addss	(%rdi,%r15,4), %xmm2
	addss	(%rcx,%rbp,4), %xmm0
	addss	(%rsi,%rbp,4), %xmm1
	addss	(%rdi,%rbp,4), %xmm2
	addl	$1, %ebp
	cmpl	%edx, %r10d
	ja	.L430
.L612:
	cmpl	%r10d, %r11d
	je	.L431
.L429:
	movl	%r11d, %edx
	subl	%r10d, %edx
	movl	%edx, %r13d
	movl	%edx, 120(%rsp)
	shrl	$2, %r13d
	leal	0(,%r13,4), %r15d
	testl	%r15d, %r15d
	je	.L432
	salq	$2, %r9
	leaq	(%rcx,%r9), %r10
	leaq	(%rsi,%r9), %r8
	addq	%rdi, %r9
	cmpl	$4, %r13d
	jbe	.L476
	leal	-5(%r13), %r12d
	xorl	%ebx, %ebx
	xorps	%xmm4, %xmm4
	shrl	$2, %r12d
	movaps	%xmm4, %xmm5
	movaps	%xmm4, %xmm7
	leaq	64(%r10), %rdx
	salq	$6, %r12
	leaq	(%rdx,%r12), %r14
	andl	$64, %r12d
	movq	%r14, 96(%rsp)
	je	.L434
	movups	16(%r8), %xmm4
	prefetcht0	272(%r10)
	movl	$4, %ebx
	movups	(%r8), %xmm5
	prefetcht0	272(%r8)
	movups	16(%r9), %xmm10
	addps	%xmm4, %xmm5
	movups	(%r9), %xmm4
	movaps	(%r10), %xmm7
	addps	%xmm10, %xmm4
	movups	32(%r8), %xmm9
	movups	32(%r9), %xmm8
	addps	16(%r10), %xmm7
	addps	%xmm9, %xmm5
	movups	48(%r8), %xmm15
	addq	$64, %r8
	addps	%xmm8, %xmm4
	movups	48(%r9), %xmm6
	addq	$64, %r9
	addps	32(%r10), %xmm7
	addps	%xmm15, %xmm5
	addps	%xmm6, %xmm4
	addps	48(%r10), %xmm7
	movq	%rdx, %r10
	addq	$64, %rdx
.L434:
	movaps	(%r10), %xmm6
	prefetcht0	272(%r8)
	leal	4(%rbx), %r14d
	movups	(%r8), %xmm9
	prefetcht0	272(%r10)
	movups	16(%r8), %xmm13
	movups	(%r9), %xmm8
	movups	16(%r9), %xmm12
	addps	%xmm13, %xmm9
	addps	16(%r10), %xmm6
	addps	%xmm12, %xmm8
	movups	32(%r8), %xmm11
	addps	%xmm5, %xmm9
	movups	32(%r9), %xmm10
	addps	%xmm7, %xmm6
	movups	48(%r9), %xmm5
	addq	$64, %r9
	addps	%xmm4, %xmm8
	movups	48(%r8), %xmm7
	addq	$64, %r8
	cmpq	96(%rsp), %rdx
	addps	%xmm11, %xmm9
	addps	32(%r10), %xmm6
	addps	%xmm10, %xmm8
	addps	%xmm7, %xmm9
	addps	48(%r10), %xmm6
	movq	%rdx, %r10
	addps	%xmm5, %xmm8
	je	.L433
	movups	16(%r8), %xmm4
	prefetcht0	272(%rdx)
	leaq	64(%rdx), %r10
	movups	(%r8), %xmm5
	prefetcht0	272(%r8)
	addl	$8, %ebx
	movaps	(%rdx), %xmm7
	addps	%xmm4, %xmm5
	movups	(%r9), %xmm4
	addps	16(%rdx), %xmm7
	movups	32(%r9), %xmm15
	addps	%xmm9, %xmm5
	movups	16(%r9), %xmm9
	addps	%xmm6, %xmm7
	movups	48(%r8), %xmm14
	addps	%xmm9, %xmm4
	movups	48(%r9), %xmm6
	addq	$64, %r9
	addps	32(%rdx), %xmm7
	addps	%xmm8, %xmm4
	movups	32(%r8), %xmm8
	addq	$64, %r8
	addps	%xmm8, %xmm5
	addps	%xmm15, %xmm4
	addps	48(%rdx), %xmm7
	subq	$-128, %rdx
	addps	%xmm14, %xmm5
	addps	%xmm6, %xmm4
	jmp	.L434
.L476:
	xorps	%xmm8, %xmm8
	xorl	%r14d, %r14d
	movaps	%xmm8, %xmm9
	movaps	%xmm8, %xmm6
.L433:
	movl	%r14d, %r12d
	movups	(%r8), %xmm5
	movl	$16, %edx
	movups	(%r9), %xmm4
	leal	1(%r14), %ebx
	notl	%r12d
	addl	%r13d, %r12d
	addps	(%r10), %xmm6
	andl	$7, %r12d
	addps	%xmm9, %xmm5
	cmpl	%ebx, %r13d
	addps	%xmm8, %xmm4
	jbe	.L614
	testl	%r12d, %r12d
	je	.L437
	cmpl	$1, %r12d
	je	.L599
	cmpl	$2, %r12d
	je	.L600
	cmpl	$3, %r12d
	je	.L601
	cmpl	$4, %r12d
	je	.L602
	cmpl	$5, %r12d
	je	.L603
	cmpl	$6, %r12d
	je	.L604
	movups	16(%r8), %xmm7
	leal	2(%r14), %ebx
	movl	$32, %edx
	movups	16(%r9), %xmm11
	addps	16(%r10), %xmm6
	addps	%xmm7, %xmm5
	addps	%xmm11, %xmm4
.L604:
	movups	(%r8,%rdx), %xmm13
	addl	$1, %ebx
	movups	(%r9,%rdx), %xmm14
	addps	%xmm13, %xmm5
	addps	%xmm14, %xmm4
	addps	(%r10,%rdx), %xmm6
	addq	$16, %rdx
.L603:
	movups	(%r8,%rdx), %xmm15
	addl	$1, %ebx
	movups	(%r9,%rdx), %xmm12
	addps	%xmm15, %xmm5
	addps	%xmm12, %xmm4
	addps	(%r10,%rdx), %xmm6
	addq	$16, %rdx
.L602:
	movups	(%r8,%rdx), %xmm9
	addl	$1, %ebx
	movups	(%r9,%rdx), %xmm8
	addps	%xmm9, %xmm5
	addps	%xmm8, %xmm4
	addps	(%r10,%rdx), %xmm6
	addq	$16, %rdx
.L601:
	movups	(%r8,%rdx), %xmm11
	addl	$1, %ebx
	movups	(%r9,%rdx), %xmm10
	addps	%xmm11, %xmm5
	addps	%xmm10, %xmm4
	addps	(%r10,%rdx), %xmm6
	addq	$16, %rdx
.L600:
	movups	(%r8,%rdx), %xmm14
	addl	$1, %ebx
	movups	(%r9,%rdx), %xmm7
	addps	%xmm14, %xmm5
	addps	%xmm7, %xmm4
	addps	(%r10,%rdx), %xmm6
	addq	$16, %rdx
.L599:
	movups	(%r8,%rdx), %xmm12
	addl	$1, %ebx
	movups	(%r9,%rdx), %xmm13
	addps	(%r10,%rdx), %xmm6
	addq	$16, %rdx
	cmpl	%ebx, %r13d
	addps	%xmm12, %xmm5
	addps	%xmm13, %xmm4
	jbe	.L614
.L437:
	movups	(%r8,%rdx), %xmm15
	addl	$8, %ebx
	movups	(%r9,%rdx), %xmm14
	addps	(%r10,%rdx), %xmm6
	addps	%xmm15, %xmm5
	movups	16(%r8,%rdx), %xmm13
	addps	%xmm14, %xmm4
	movups	16(%r9,%rdx), %xmm12
	addps	16(%r10,%rdx), %xmm6
	addps	%xmm13, %xmm5
	movups	32(%r8,%rdx), %xmm11
	addps	%xmm12, %xmm4
	movups	32(%r9,%rdx), %xmm10
	addps	32(%r10,%rdx), %xmm6
	addps	%xmm11, %xmm5
	movups	48(%r8,%rdx), %xmm7
	addps	%xmm10, %xmm4
	movups	48(%r9,%rdx), %xmm9
	addps	48(%r10,%rdx), %xmm6
	addps	%xmm7, %xmm5
	movups	64(%r8,%rdx), %xmm8
	addps	%xmm9, %xmm4
	movups	64(%r9,%rdx), %xmm15
	addps	64(%r10,%rdx), %xmm6
	addps	%xmm8, %xmm5
	movups	80(%r8,%rdx), %xmm14
	addps	%xmm15, %xmm4
	movups	80(%r9,%rdx), %xmm13
	addps	80(%r10,%rdx), %xmm6
	addps	%xmm14, %xmm5
	movups	96(%r8,%rdx), %xmm12
	addps	%xmm13, %xmm4
	movups	96(%r9,%rdx), %xmm11
	addps	96(%r10,%rdx), %xmm6
	addps	%xmm12, %xmm5
	movups	112(%r8,%rdx), %xmm10
	addps	%xmm11, %xmm4
	movups	112(%r9,%rdx), %xmm7
	addps	112(%r10,%rdx), %xmm6
	subq	$-128, %rdx
	cmpl	%ebx, %r13d
	addps	%xmm10, %xmm5
	addps	%xmm7, %xmm4
	ja	.L437
.L614:
	haddps	%xmm4, %xmm4
	addl	%r15d, %ebp
	cmpl	%r15d, 120(%rsp)
	haddps	%xmm5, %xmm5
	haddps	%xmm6, %xmm6
	haddps	%xmm4, %xmm4
	haddps	%xmm5, %xmm5
	haddps	%xmm6, %xmm6
	addss	%xmm4, %xmm2
	addss	%xmm5, %xmm1
	addss	%xmm6, %xmm0
	je	.L431
.L432:
	movl	%ebp, %ebx
	movslq	%ebp, %r8
	movl	$1, %edx
	leal	1(%rbp), %r15d
	notl	%ebx
	salq	$2, %r8
	leaq	(%rcx,%r8), %r10
	addl	%r11d, %ebx
	leaq	(%rsi,%r8), %r9
	andl	$3, %ebx
	addq	%rdi, %r8
	cmpl	%r15d, %r11d
	addss	(%r10), %xmm0
	addss	(%r9), %xmm1
	addss	(%r8), %xmm2
	jle	.L431
	testl	%ebx, %ebx
	je	.L436
	cmpl	$1, %ebx
	je	.L597
	cmpl	$2, %ebx
	.p2align 4,,3
	je	.L598
	addss	4(%r10), %xmm0
	movl	$2, %edx
	addss	4(%r9), %xmm1
	addss	4(%r8), %xmm2
.L598:
	addss	(%r10,%rdx,4), %xmm0
	addss	(%r9,%rdx,4), %xmm1
	addss	(%r8,%rdx,4), %xmm2
	addq	$1, %rdx
.L597:
	addss	(%r10,%rdx,4), %xmm0
	addss	(%r9,%rdx,4), %xmm1
	addss	(%r8,%rdx,4), %xmm2
	addq	$1, %rdx
	leal	0(%rbp,%rdx), %ebx
	cmpl	%ebx, %r11d
	jle	.L431
.L436:
	addss	(%r10,%rdx,4), %xmm0
	leaq	1(%rdx), %r14
	addss	(%r9,%rdx,4), %xmm1
	leaq	2(%rdx), %r12
	addss	(%r8,%rdx,4), %xmm2
	leaq	3(%rdx), %rbx
	addq	$4, %rdx
	addss	(%r10,%r14,4), %xmm0
	leal	0(%rbp,%rdx), %r13d
	addss	(%r9,%r14,4), %xmm1
	cmpl	%r13d, %r11d
	addss	(%r8,%r14,4), %xmm2
	addss	(%r10,%r12,4), %xmm0
	addss	(%r9,%r12,4), %xmm1
	addss	(%r8,%r12,4), %xmm2
	addss	(%r10,%rbx,4), %xmm0
	addss	(%r9,%rbx,4), %xmm1
	addss	(%r8,%rbx,4), %xmm2
	jg	.L436
.L431:
	cvtsi2ss	%r11d, %xmm4
	movss	.LC33(%rip), %xmm6
	movl	%r11d, %r15d
	leaq	16(%rdi), %r14
	shrl	$2, %r15d
	leaq	16(%rsi), %r13
	cmpq	%rsi, %r14
	leal	0(,%r15,4), %r8d
	divss	%xmm4, %xmm6
	movl	%r8d, 96(%rsp)
	setb	%r8b
	leaq	16(%rcx), %rbx
	cmpq	%rdi, %r13
	setb	%bpl
	orl	%ebp, %r8d
	cmpq	%rcx, %r14
	setb	%bpl
	cmpq	%rdi, %rbx
	setb	%r12b
	orl	%r12d, %ebp
	cmpl	$4, %r11d
	seta	%r9b
	cmpq	%rsi, %rbx
	setb	%r10b
	cmpq	%rcx, %r13
	setb	%dl
	orl	%edx, %r10d
	andl	%r10d, %r9d
	andl	%r9d, %ebp
	testb	%bpl, %r8b
	mulss	%xmm6, %xmm0
	mulss	%xmm6, %xmm1
	mulss	%xmm6, %xmm2
	je	.L477
	movl	96(%rsp), %r9d
	testl	%r9d, %r9d
	je	.L477
	movaps	%xmm0, %xmm7
	movaps	%xmm1, %xmm9
	movaps	%xmm2, %xmm8
	cmpl	$4, %r15d
	shufps	$0, %xmm7, %xmm7
	shufps	$0, %xmm9, %xmm9
	shufps	$0, %xmm8, %xmm8
	movaps	%xmm7, %xmm6
	movaps	%xmm9, %xmm5
	movaps	%xmm8, %xmm4
	jbe	.L478
	leal	-5(%r15), %r10d
	movq	%rdi, %r9
	movq	%rcx, %r8
	leaq	192(%rcx), %rdx
	shrl	$2, %r10d
	xorl	%r14d, %r14d
	salq	$6, %r10
	leaq	64(%rcx,%r10), %rbx
	movq	%rsi, %r10
.L441:
	movups	(%r8), %xmm12
	prefetcht0	(%rdx)
	prefetcht0	(%rdx)
	addl	$4, %r14d
	addq	$64, %rdx
	subps	%xmm6, %xmm12
	movups	%xmm12, (%r8)
	movups	(%r10), %xmm11
	subps	%xmm5, %xmm11
	movups	%xmm11, (%r10)
	movups	(%r9), %xmm10
	subps	%xmm4, %xmm10
	movups	%xmm10, (%r9)
	movups	16(%r8), %xmm7
	subps	%xmm6, %xmm7
	movups	%xmm7, 16(%r8)
	movups	16(%r10), %xmm9
	subps	%xmm5, %xmm9
	movups	%xmm9, 16(%r10)
	movups	16(%r9), %xmm8
	subps	%xmm4, %xmm8
	movups	%xmm8, 16(%r9)
	movups	32(%r8), %xmm15
	subps	%xmm6, %xmm15
	movups	%xmm15, 32(%r8)
	movups	32(%r10), %xmm14
	subps	%xmm5, %xmm14
	movups	%xmm14, 32(%r10)
	movups	32(%r9), %xmm13
	subps	%xmm4, %xmm13
	movups	%xmm13, 32(%r9)
	movups	48(%r8), %xmm12
	subps	%xmm6, %xmm12
	movups	%xmm12, 48(%r8)
	addq	$64, %r8
	movups	48(%r10), %xmm11
	subps	%xmm5, %xmm11
	movups	%xmm11, 48(%r10)
	addq	$64, %r10
	movups	48(%r9), %xmm10
	subps	%xmm4, %xmm10
	movups	%xmm10, 48(%r9)
	addq	$64, %r9
	cmpq	%rbx, %r8
	jne	.L441
	movq	%r9, %rbp
	movq	%r10, %r12
	movq	%r8, %r13
.L440:
	movups	(%r8), %xmm15
	leal	1(%r14), %ebx
	movl	%r14d, %edx
	notl	%edx
	subps	%xmm6, %xmm15
	addl	%r15d, %edx
	andl	$3, %edx
	cmpl	%ebx, %r15d
	movl	%edx, 120(%rsp)
	movl	$16, %edx
	movups	%xmm15, 0(%r13)
	movups	(%r10), %xmm14
	subps	%xmm5, %xmm14
	movups	%xmm14, (%r12)
	movups	(%r9), %xmm13
	subps	%xmm4, %xmm13
	movups	%xmm13, 0(%rbp)
	jbe	.L615
	cmpl	$0, 120(%rsp)
	je	.L444
	cmpl	$1, 120(%rsp)
	je	.L595
	cmpl	$2, 120(%rsp)
	je	.L596
	movups	16(%r8), %xmm14
	leal	2(%r14), %ebx
	movl	$32, %edx
	subps	%xmm6, %xmm14
	movups	%xmm14, 16(%r13)
	movups	16(%r10), %xmm7
	subps	%xmm5, %xmm7
	movups	%xmm7, 16(%r12)
	movups	16(%r9), %xmm11
	subps	%xmm4, %xmm11
	movups	%xmm11, 16(%rbp)
.L596:
	movups	(%r8,%rdx), %xmm15
	addl	$1, %ebx
	subps	%xmm6, %xmm15
	movups	%xmm15, 0(%r13,%rdx)
	movups	(%r10,%rdx), %xmm12
	subps	%xmm5, %xmm12
	movups	%xmm12, (%r12,%rdx)
	movups	(%r9,%rdx), %xmm13
	subps	%xmm4, %xmm13
	movups	%xmm13, 0(%rbp,%rdx)
	addq	$16, %rdx
.L595:
	movups	(%r8,%rdx), %xmm10
	addl	$1, %ebx
	subps	%xmm6, %xmm10
	movups	%xmm10, 0(%r13,%rdx)
	movups	(%r10,%rdx), %xmm9
	subps	%xmm5, %xmm9
	movups	%xmm9, (%r12,%rdx)
	movups	(%r9,%rdx), %xmm8
	subps	%xmm4, %xmm8
	movups	%xmm8, 0(%rbp,%rdx)
	addq	$16, %rdx
	cmpl	%ebx, %r15d
	jbe	.L615
.L444:
	movups	(%r8,%rdx), %xmm7
	addl	$4, %ebx
	subps	%xmm6, %xmm7
	movups	%xmm7, 0(%r13,%rdx)
	movups	(%r10,%rdx), %xmm9
	subps	%xmm5, %xmm9
	movups	%xmm9, (%r12,%rdx)
	movups	(%r9,%rdx), %xmm8
	subps	%xmm4, %xmm8
	movups	%xmm8, 0(%rbp,%rdx)
	movups	16(%r8,%rdx), %xmm15
	subps	%xmm6, %xmm15
	movups	%xmm15, 16(%r13,%rdx)
	movups	16(%r10,%rdx), %xmm14
	subps	%xmm5, %xmm14
	movups	%xmm14, 16(%r12,%rdx)
	movups	16(%r9,%rdx), %xmm13
	subps	%xmm4, %xmm13
	movups	%xmm13, 16(%rbp,%rdx)
	movups	32(%r8,%rdx), %xmm12
	subps	%xmm6, %xmm12
	movups	%xmm12, 32(%r13,%rdx)
	movups	32(%r10,%rdx), %xmm11
	subps	%xmm5, %xmm11
	movups	%xmm11, 32(%r12,%rdx)
	movups	32(%r9,%rdx), %xmm10
	subps	%xmm4, %xmm10
	movups	%xmm10, 32(%rbp,%rdx)
	movups	48(%r8,%rdx), %xmm7
	subps	%xmm6, %xmm7
	movups	%xmm7, 48(%r13,%rdx)
	movups	48(%r10,%rdx), %xmm9
	subps	%xmm5, %xmm9
	movups	%xmm9, 48(%r12,%rdx)
	movups	48(%r9,%rdx), %xmm8
	subps	%xmm4, %xmm8
	movups	%xmm8, 48(%rbp,%rdx)
	addq	$64, %rdx
	cmpl	%ebx, %r15d
	ja	.L444
.L615:
	movl	96(%rsp), %ebx
	cmpl	%r11d, %ebx
	je	.L442
.L439:
	movslq	%ebx, %r8
	movl	%ebx, %ebp
	movl	$1, %edx
	leal	1(%rbx), %r15d
	salq	$2, %r8
	notl	%ebp
	leaq	(%rcx,%r8), %r10
	addl	%r11d, %ebp
	movss	(%r10), %xmm4
	leaq	(%rsi,%r8), %r9
	addq	%rdi, %r8
	andl	$3, %ebp
	cmpl	%r15d, %r11d
	subss	%xmm0, %xmm4
	movss	%xmm4, (%r10)
	movss	(%r9), %xmm6
	subss	%xmm1, %xmm6
	movss	%xmm6, (%r9)
	movss	(%r8), %xmm5
	subss	%xmm2, %xmm5
	movss	%xmm5, (%r8)
	jle	.L442
	testl	%ebp, %ebp
	je	.L443
	cmpl	$1, %ebp
	je	.L593
	cmpl	$2, %ebp
	.p2align 4,,3
	je	.L594
	movss	4(%r10), %xmm13
	movl	$2, %edx
	subss	%xmm0, %xmm13
	movss	%xmm13, 4(%r10)
	movss	4(%r9), %xmm14
	subss	%xmm1, %xmm14
	movss	%xmm14, 4(%r9)
	movss	4(%r8), %xmm7
	subss	%xmm2, %xmm7
	movss	%xmm7, 4(%r8)
.L594:
	movss	(%r10,%rdx,4), %xmm8
	subss	%xmm0, %xmm8
	movss	%xmm8, (%r10,%rdx,4)
	movss	(%r9,%rdx,4), %xmm15
	subss	%xmm1, %xmm15
	movss	%xmm15, (%r9,%rdx,4)
	movss	(%r8,%rdx,4), %xmm12
	subss	%xmm2, %xmm12
	movss	%xmm12, (%r8,%rdx,4)
	addq	$1, %rdx
.L593:
	movss	(%r10,%rdx,4), %xmm10
	subss	%xmm0, %xmm10
	movss	%xmm10, (%r10,%rdx,4)
	movss	(%r9,%rdx,4), %xmm6
	subss	%xmm1, %xmm6
	movss	%xmm6, (%r9,%rdx,4)
	movss	(%r8,%rdx,4), %xmm9
	subss	%xmm2, %xmm9
	movss	%xmm9, (%r8,%rdx,4)
	addq	$1, %rdx
	leal	(%rbx,%rdx), %ebp
	cmpl	%ebp, %r11d
	jle	.L442
.L443:
	movss	(%r10,%rdx,4), %xmm4
	leaq	1(%rdx), %rbp
	leaq	2(%rdx), %r14
	subss	%xmm0, %xmm4
	leaq	3(%rdx), %r12
	movss	%xmm4, (%r10,%rdx,4)
	movss	(%r9,%rdx,4), %xmm6
	subss	%xmm1, %xmm6
	movss	%xmm6, (%r9,%rdx,4)
	movss	(%r8,%rdx,4), %xmm5
	subss	%xmm2, %xmm5
	movss	%xmm5, (%r8,%rdx,4)
	addq	$4, %rdx
	movss	(%r10,%rbp,4), %xmm12
	leal	(%rbx,%rdx), %r13d
	cmpl	%r13d, %r11d
	subss	%xmm0, %xmm12
	movss	%xmm12, (%r10,%rbp,4)
	movss	(%r9,%rbp,4), %xmm11
	subss	%xmm1, %xmm11
	movss	%xmm11, (%r9,%rbp,4)
	movss	(%r8,%rbp,4), %xmm10
	subss	%xmm2, %xmm10
	movss	%xmm10, (%r8,%rbp,4)
	movss	(%r10,%r14,4), %xmm7
	subss	%xmm0, %xmm7
	movss	%xmm7, (%r10,%r14,4)
	movss	(%r9,%r14,4), %xmm9
	subss	%xmm1, %xmm9
	movss	%xmm9, (%r9,%r14,4)
	movss	(%r8,%r14,4), %xmm8
	subss	%xmm2, %xmm8
	movss	%xmm8, (%r8,%r14,4)
	movss	(%r10,%r12,4), %xmm15
	subss	%xmm0, %xmm15
	movss	%xmm15, (%r10,%r12,4)
	movss	(%r9,%r12,4), %xmm14
	subss	%xmm1, %xmm14
	movss	%xmm14, (%r9,%r12,4)
	movss	(%r8,%r12,4), %xmm13
	subss	%xmm2, %xmm13
	movss	%xmm13, (%r8,%r12,4)
	jg	.L443
.L442:
	movq	mc_time+8(%rip), %rbx
	movq	simufiles(%rip), %rbp
	subq	%rax, %rbx
.L446:
	xorl	%r12d, %r12d
.L448:
	movl	%r12d, %r11d
	movq	%rbx, %rdx
	movl	$3, %eax
	movss	(%rdi,%r11,4), %xmm2
	movss	(%rsi,%r11,4), %xmm1
	movq	%rbp, %rdi
	movss	(%rcx,%r11,4), %xmm0
	movl	$.LC29, %esi
	movsd	%xmm3, 16(%rsp)
	cvtps2pd	%xmm0, %xmm0
	cvtps2pd	%xmm2, %xmm2
	cvtps2pd	%xmm1, %xmm1
	call	gzprintf
	leal	1(%r12), %eax
	movsd	16(%rsp), %xmm3
	cmpl	N(%rip), %eax
	jae	.L447
	movq	dots(%rip), %rdi
	movq	%rbx, %rdx
	movq	dots+16(%rip), %rsi
	movq	dots+8(%rip), %rcx
	movss	(%rdi,%rax,4), %xmm6
	movq	%rbp, %rdi
	movss	(%rsi,%rax,4), %xmm5
	movl	$.LC29, %esi
	cvtps2pd	%xmm6, %xmm0
	movss	(%rcx,%rax,4), %xmm3
	movl	$3, %eax
	cvtps2pd	%xmm5, %xmm2
	cvtps2pd	%xmm3, %xmm1
	call	gzprintf
	leal	2(%r12), %eax
	cmpl	N(%rip), %eax
	movsd	16(%rsp), %xmm3
	jae	.L447
	movq	dots(%rip), %rdx
	movl	$.LC29, %esi
	movq	%rbp, %rdi
	movq	dots+16(%rip), %r8
	movq	dots+8(%rip), %r9
	movss	(%rdx,%rax,4), %xmm2
	movq	%rbx, %rdx
	movss	(%r8,%rax,4), %xmm1
	cvtps2pd	%xmm2, %xmm0
	movss	(%r9,%rax,4), %xmm4
	movl	$3, %eax
	cvtps2pd	%xmm1, %xmm2
	movsd	%xmm3, 16(%rsp)
	cvtps2pd	%xmm4, %xmm1
	call	gzprintf
	leal	3(%r12), %eax
	movsd	16(%rsp), %xmm3
	cmpl	N(%rip), %eax
	jae	.L447
	movq	dots(%rip), %rcx
	movq	%rbx, %rdx
	movl	$.LC29, %esi
	movq	dots+16(%rip), %r10
	movq	%rbp, %rdi
	addl	$4, %r12d
	movq	dots+8(%rip), %r14
	movss	(%rcx,%rax,4), %xmm11
	movss	(%r10,%rax,4), %xmm5
	cvtps2pd	%xmm11, %xmm0
	movss	(%r14,%rax,4), %xmm3
	movl	$3, %eax
	cvtps2pd	%xmm5, %xmm2
	cvtps2pd	%xmm3, %xmm1
	call	gzprintf
	cmpl	N(%rip), %r12d
	movsd	16(%rsp), %xmm3
	jae	.L447
	movq	dots(%rip), %rcx
	movq	dots+8(%rip), %rsi
	movq	dots+16(%rip), %rdi
	jmp	.L448
.L475:
	xorps	%xmm2, %xmm2
	xorl	%ebp, %ebp
	movaps	%xmm2, %xmm1
	movaps	%xmm2, %xmm0
	jmp	.L429
.L478:
	movq	%rdi, %rbp
	movq	%rdi, %r9
	movq	%rsi, %r12
	movq	%rsi, %r10
	movq	%rcx, %r13
	movq	%rcx, %r8
	xorl	%r14d, %r14d
	jmp	.L440
.L428:
	movq	mc_time+8(%rip), %rbx
	movq	simufiles(%rip), %rbp
	subq	%rax, %rbx
	testl	%r11d, %r11d
	je	.L447
	movq	dots(%rip), %rcx
	movq	dots+8(%rip), %rsi
	movq	dots+16(%rip), %rdi
	jmp	.L446
.L631:
	movl	$.LC31, %edi
	movl	$24, %edx
	movl	$1, %esi
	movq	stderr(%rip), %rcx
	call	fwrite
	movl	$-8, %edi
	call	exit
.L477:
	xorl	%ebx, %ebx
	jmp	.L439
.L447:
	movl	124(%rsp), %eax
	movl	$.LC34, %esi
	movsd	%xmm3, 16(%rsp)
	movl	168(%rsp), %ebx
	movq	simufiles+8(%rip), %rdi
	cvtsi2ssq	%rax, %xmm11
	movl	$1, %eax
	cvtsi2ssq	%rbx, %xmm10
	divss	%xmm11, %xmm10
	unpcklps	%xmm10, %xmm10
	cvtps2pd	%xmm10, %xmm0
	call	fprintf
	movl	contacts(%rip), %ecx
	movl	$.LC35, %esi
	movl	$1, %eax
	movl	contacts_loc(%rip), %r8d
	movq	mc_time+8(%rip), %rdx
	movq	simufiles+16(%rip), %rdi
	cvtsi2sd	%ecx, %xmm4
	cvtsi2sd	%r8d, %xmm0
	subq	mc_time(%rip), %rdx
	mulsd	beta_uniform(%rip), %xmm4
	mulsd	beta_localized(%rip), %xmm0
	addsd	%xmm4, %xmm0
	call	fprintf
	movl	$1, 124(%rsp)
	movsd	16(%rsp), %xmm3
	movl	$0, 168(%rsp)
	jmp	.L427
	.cfi_endproc
.LFE636:
	.size	simulazione, .-simulazione
	.comm	beta_localized,8,16
	.comm	alfa_localized,4,16
	.comm	beta_uniform,8,16
	.comm	alfa_uniform,4,16
	.comm	sigma,4,16
	.comm	lambda,4,16
	.comm	simufiles,56,16
	.comm	mc_time,16,16
	.comm	locnum,4,4
	.comm	locmask,8,16
	.comm	lpl_index,8,16
	.comm	lpl,8,16
	.comm	dots,24,16
	.comm	buffer,8,8
	.comm	N,4,16
	.local	dsfmt
	.comm	dsfmt,3088,32
	.local	D
	.comm	D,4,16
	.local	contacts_loc
	.comm	contacts_loc,4,16
	.local	contacts
	.comm	contacts,4,16
	.local	comp_hc
	.comm	comp_hc,16,16
	.local	comp_sb
	.comm	comp_sb,16,16
	.local	comp_sp
	.comm	comp_sp,16,16
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC0:
	.long	981668463
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC1:
	.long	-2147483648
	.long	0
	.long	0
	.long	0
	.section	.rodata.cst4
	.align 4
.LC2:
	.long	1056964608
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC12:
	.long	-1431655765
	.long	1073392298
	.align 8
.LC13:
	.long	0
	.long	1074266112
	.align 8
.LC14:
	.long	1413754136
	.long	1076437499
	.align 8
.LC15:
	.long	1224548090
	.long	1073156980
	.align 8
.LC16:
	.long	858993459
	.long	1071854387
	.align 8
.LC20:
	.long	0
	.long	1072693248
	.align 8
.LC21:
	.long	0
	.long	-1074790400
	.align 8
.LC22:
	.long	-1717986918
	.long	1069128089
	.section	.rodata.cst16
	.align 16
.LC23:
	.long	2143289344
	.long	2143289344
	.long	2143289344
	.long	2143289344
	.section	.rodata.cst8
	.align 8
.LC32:
	.long	0
	.long	2146959360
	.section	.rodata.cst4
	.align 4
.LC33:
	.long	1065353216
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
