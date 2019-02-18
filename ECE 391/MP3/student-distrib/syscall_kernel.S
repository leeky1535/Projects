# Kernel side of syscall interface

#define ASM 1
#include "x86_desc.h"

.globl syscall_0

syscall_0:
	subl $1, %eax #Syscalls start at 1
	cmpl $10, %eax #Check syscall range
	jae invalid_syscall
	
	pushfl
	pushl %edi #Save all registers. This prevents info from leaking the kernal
	pushl %esi
	pushl %edx
	pushl %ecx
	pushl %ebx

	call *syscall_jump_table(,%eax,4) #Restore all registers

	popl %ebx
	popl %ecx
	popl %edx
	popl %esi
	popl %edi
	
	popfl
	iret
	
invalid_syscall:
	movl $-1, %eax #Return -1 to signify error
	iret

# Assembly wrappers for syscall

syscall_jump_table:
	.long _halt, _execute, _read, _write, _open, _close, _getargs, _vidmap, _set_handler, _sigreturn

_halt:
	pushl %ebx
	call halt
	addl $4, %esp
	ret

_execute:
	pushl %ebx
	call execute
	addl $4, %esp
	ret

_read:
	pushl %edx
	pushl %ecx
	pushl %ebx
	call read
	addl $12, %esp
	ret

_write:
	pushl %edx
	pushl %ecx
	pushl %ebx
	call write
	addl $12, %esp
	ret

_open:
	pushl %ebx
	call open
	addl $4, %esp
	ret

_close:
	pushl %ebx
	call close
	addl $4, %esp
	ret

_getargs:
	pushl %ecx
	pushl %ebx
	call getargs
	addl $8, %esp
	ret

_vidmap:
	pushl %ebx
	call vidmap
	addl $4, %esp
	ret

_set_handler:
	pushl %ecx
	pushl %ebx
	call set_handler
	addl $8, %esp
	ret

_sigreturn:
	call sigreturn
	ret
