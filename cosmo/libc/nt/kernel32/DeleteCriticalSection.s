.include "o/libc/nt/codegen.inc"
.imp	kernel32,__imp_DeleteCriticalSection,DeleteCriticalSection,0

	.text.windows
DeleteCriticalSection:
	push	%rbp
	mov	%rsp,%rbp
	.profilable
	mov	%rdi,%rcx
	sub	$32,%rsp
	call	*__imp_DeleteCriticalSection(%rip)
	leave
	ret
	.endfn	DeleteCriticalSection,globl
	.previous
