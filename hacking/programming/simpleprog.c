#include <stdio.h>

int main() {
    int i;
    for (i = 0; i < 10; i++) {
        puts("Hello, world!\n");
    }
    return 0;
}

// use objdump to examine compiled binary, 
// specifically searching for the main block and 20 lines after it

// The columns of objdump correspond to the following
// MEMORY_ADDRESS | MACHINE_LANGUAGE_INSTRUCTION | ASSEMBLY LANGUAGE


// lkrych@LKRYCH-M-W49D ~/network_programming/hacking/programming (master) $ objdump -D simple | grep _main: -A 20
// _main:
// 100000f40:	55 	pushq	%rbp
// 100000f41:	48 89 e5 	movq	%rsp, %rbp
// 100000f44:	48 83 ec 10 	subq	$16, %rsp
// 100000f48:	c7 45 fc 00 00 00 00 	movl	$0, -4(%rbp)
// 100000f4f:	c7 45 f8 00 00 00 00 	movl	$0, -8(%rbp)
// 100000f56:	83 7d f8 0a 	cmpl	$10, -8(%rbp)
// 100000f5a:	0f 8d 1d 00 00 00 	jge	29 <_main+0x3d>
// 100000f60:	48 8d 3d 3f 00 00 00 	leaq	63(%rip), %rdi
// 100000f67:	e8 1a 00 00 00 	callq	26 <dyld_stub_binder+0x100000f86>
// 100000f6c:	89 45 f4 	movl	%eax, -12(%rbp)
// 100000f6f:	8b 45 f8 	movl	-8(%rbp), %eax
// 100000f72:	83 c0 01 	addl	$1, %eax
// 100000f75:	89 45 f8 	movl	%eax, -8(%rbp)
// 100000f78:	e9 d9 ff ff ff 	jmp	-39 <_main+0x16>
// 100000f7d:	31 c0 	xorl	%eax, %eax
// 100000f7f:	48 83 c4 10 	addq	$16, %rsp
// 100000f83:	5d 	popq	%rbp
// 100000f84:	c3 	retq

// To show the intel assembly language syntax
// lkrych@LKRYCH-M-W49D ~/network_programming/hacking/programming (master) $ objdump -x86-asm-syntax intel -D simple | grep _main: -A 20
// _main:
// 100000f40:	55 	push	rbp
// 100000f41:	48 89 e5 	mov	rbp, rsp
// 100000f44:	48 83 ec 10 	sub	rsp, 16
// 100000f48:	c7 45 fc 00 00 00 00 	mov	dword ptr [rbp - 4], 0
// 100000f4f:	c7 45 f8 00 00 00 00 	mov	dword ptr [rbp - 8], 0
// 100000f56:	83 7d f8 0a 	cmp	dword ptr [rbp - 8], 10
// 100000f5a:	0f 8d 1d 00 00 00 	jge	29 <_main+0x3d>
// 100000f60:	48 8d 3d 3f 00 00 00 	lea	rdi, [rip + 63]
// 100000f67:	e8 1a 00 00 00 	call	26 <dyld_stub_binder+0x100000f86>
// 100000f6c:	89 45 f4 	mov	dword ptr [rbp - 12], eax
// 100000f6f:	8b 45 f8 	mov	eax, dword ptr [rbp - 8]
// 100000f72:	83 c0 01 	add	eax, 1
// 100000f75:	89 45 f8 	mov	dword ptr [rbp - 8], eax
// 100000f78:	e9 d9 ff ff ff 	jmp	-39 <_main+0x16>
// 100000f7d:	31 c0 	xor	eax, eax
// 100000f7f:	48 83 c4 10 	add	rsp, 16
// 100000f83:	5d 	pop	rbp
// 100000f84:	c3 	ret
// Disassembly of section __TEXT,__stubs:

// compile with -g flag to include extra debugging information and give lldb
// access to the source code

//output created with: disassemble --name main in lldb

// a.out[0x100000f40] <+0>:  push   rbp
// a.out[0x100000f41] <+1>:  mov    rbp, rsp
// a.out[0x100000f44] <+4>:  sub    rsp, 0x10
// a.out[0x100000f48] <+8>:  mov    dword ptr [rbp - 0x4], 0x0
// a.out[0x100000f4f] <+15>: mov    dword ptr [rbp - 0x8], 0x0
// a.out[0x100000f56] <+22>: cmp    dword ptr [rbp - 0x8], 0xa #check if i > 10
// a.out[0x100000f5a] <+26>: jge    0x100000f7d               ; <+61> at simpleprog.c #jump if it is
// a.out[0x100000f60] <+32>: lea    rdi, [rip + 0x3f]         ; "Hello, world!\n"     #otherwise print hello world
// a.out[0x100000f67] <+39>: call   0x100000f86               ; symbol stub for: puts #ditto above
// a.out[0x100000f6c] <+44>: mov    dword ptr [rbp - 0xc], eax 
// a.out[0x100000f6f] <+47>: mov    eax, dword ptr [rbp - 0x8] #increment i by 1
// a.out[0x100000f72] <+50>: add    eax, 0x1                   #increment i by 1
// a.out[0x100000f75] <+53>: mov    dword ptr [rbp - 0x8], eax #increment i by 1
// a.out[0x100000f78] <+56>: jmp    0x100000f56               ; <+22> at simpleprog.c:5:19 #go back to beginning of loop
// a.out[0x100000f7d] <+61>: xor    eax, eax
// a.out[0x100000f7f] <+63>: add    rsp, 0x10
// a.out[0x100000f83] <+67>: pop    rbp
// a.out[0x100000f84] <+68>: ret   