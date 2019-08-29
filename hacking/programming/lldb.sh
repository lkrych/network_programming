lldb simple
# Current executable set to 'simple' (x86_64).

# we are now ready to set a breakpoint. The purpose of this lldb session is to inspect our registers
# first we need to set a breakpoint at the main function

# (lldb) b main
# Breakpoint 1: where = simple`main, address = 0x0000000100000f40
# (lldb) run
# Process 15177 launched: '\blahblahblahblahblah' (x86_64)
# Process 15177 stopped
# * thread #1, queue = 'com.apple.main-thread', stop reason = breakpoint 1.1
#     frame #0: 0x0000000100000f40 simple`main
# simple`main:
# ->  0x100000f40 <+0>: pushq  %rbp
#     0x100000f41 <+1>: movq   %rsp, %rbp
#     0x100000f44 <+4>: subq   $0x10, %rsp
#     0x100000f48 <+8>: movl   $0x0, -0x4(%rbp)
# Target 0: (simple) stopped.

# by default shows the next Assembly steps

# (lldb) register read
# General Purpose Registers:
#        rax = 0x0000000100000f40  simple`main
#        rbx = 0x0000000000000000
#        rcx = 0x00007ffeefbff350
#        rdx = 0x00007ffeefbff180
#        rdi = 0x0000000000000001
#        rsi = 0x00007ffeefbff170
#        rbp = 0x00007ffeefbff160
#        rsp = 0x00007ffeefbff158
#         r8 = 0x0000000000000000
#         r9 = 0x0000000000000000
#        r10 = 0x0000000000000000
#        r11 = 0x0000000000000000
#        r12 = 0x0000000000000000
#        r13 = 0x0000000000000000
#        r14 = 0x0000000000000000
#        r15 = 0x0000000000000000
#        rip = 0x0000000100000f40  simple`main
#     rflags = 0x0000000000000246
#         cs = 0x000000000000002b
#         fs = 0x0000000000000000
#         gs = 0x0000000000000000

# RAX, RCX, RDX, RBX -> General-purpose registers: Accumulator, Counter, Data, Base
# these registers act as temporary variables for the CPU when it is executing machine instructions

# RSP, RBP, RSI, RDI -> General-purpose registers, but known as Pointers and Indexes. The first two are known
# as pointers because they store 32-bit addresses, which essentially point to that location in memory
# the last two registers are used to point to the source and destination when data needs to be read from or written to

# RIP -> Instruction Pointer Register, points to the current instruction the processor is reading

# RFLAGS -> consists of several bit flags that are used for comparisons and memory segmentations

# (lldb) quit
# Quitting LLDB will kill one or more processes. Do you really want to proceed: [Y/n] y 

lldb simple

#set assembly flavor to intel in lldb

# Current executable set to 'simple' (x86_64).
# (lldb) b main
# Breakpoint 1: where = simple`main, address = 0x0000000100000f40
# (lldb) settings set target.x86-disassembly-flavor intel
# (lldb) run
# Process 18451 launched: '/Users/lkrych/network_programming/hacking/programming/simple' (x86_64)
# Process 18451 stopped
# * thread #1, queue = 'com.apple.main-thread', stop reason = breakpoint 1.1
#     frame #0: 0x0000000100000f40 simple`main
# simple`main:
# ->  0x100000f40 <+0>: push   rbp
#     0x100000f41 <+1>: mov    rbp, rsp
#     0x100000f44 <+4>: sub    rsp, 0x10
#     0x100000f48 <+8>: mov    dword ptr [rbp - 0x4], 0x0
# Target 0: (simple) stopped.

#GET FANCY WITH THE LLDB

# compile with the g flag to give access to lldb

# lkrych@LKRYCH-M-W49D ~/blahblahblahblah (master) $ gcc -g simpleprog.c
# lkrych@LKRYCH-M-W49D ~/blahblahblahblah (master) $ lldb ./a.out
# (lldb) target create "./a.out"
# Current executable set to './a.out' (x86_64).
# (lldb) settings set target.x86-disassembly-flavor intel
# (lldb) list
#    3    int main() {
#    4        int i;
#    5        for (i = 0; i < 10; i++) {
#    6            puts("Hello, world!\n");
#    7        }
#    8        return 0;
#    9    }
#    10  
#    11   // use objdump to examine compiled binary, 
#    12   // specifically searching for the main block and 20 lines after it
# (lldb) disassemble --name main 
# a.out`main:
# a.out[0x100000f40] <+0>:  push   rbp
# a.out[0x100000f41] <+1>:  mov    rbp, rsp
# a.out[0x100000f44] <+4>:  sub    rsp, 0x10
# a.out[0x100000f48] <+8>:  mov    dword ptr [rbp - 0x4], 0x0
# a.out[0x100000f4f] <+15>: mov    dword ptr [rbp - 0x8], 0x0
# a.out[0x100000f56] <+22>: cmp    dword ptr [rbp - 0x8], 0xa
# a.out[0x100000f5a] <+26>: jge    0x100000f7d               ; <+61> at simpleprog.c
# a.out[0x100000f60] <+32>: lea    rdi, [rip + 0x3f]         ; "Hello, world!\n"
# a.out[0x100000f67] <+39>: call   0x100000f86               ; symbol stub for: puts
# a.out[0x100000f6c] <+44>: mov    dword ptr [rbp - 0xc], eax
# a.out[0x100000f6f] <+47>: mov    eax, dword ptr [rbp - 0x8]
# a.out[0x100000f72] <+50>: add    eax, 0x1
# a.out[0x100000f75] <+53>: mov    dword ptr [rbp - 0x8], eax
# a.out[0x100000f78] <+56>: jmp    0x100000f56               ; <+22> at simpleprog.c:5:19
# a.out[0x100000f7d] <+61>: xor    eax, eax
# a.out[0x100000f7f] <+63>: add    rsp, 0x10
# a.out[0x100000f83] <+67>: pop    rbp
# a.out[0x100000f84] <+68>: ret    

# (lldb) b main
# Breakpoint 1: where = a.out`main + 15 at simpleprog.c:5:12, address = 0x0000000100000f4f
# (lldb) run
# Process 21251 launched: '/blahblahblahblahblah' (x86_64)
# Process 21251 stopped
# * thread #1, queue = 'com.apple.main-thread', stop reason = breakpoint 1.1
#     frame #0: 0x0000000100000f4f a.out`main at simpleprog.c:5:12
#    2   
#    3    int main() {
#    4        int i;
# -> 5        for (i = 0; i < 10; i++) {
#    6            puts("Hello, world!\n");
#    7        }
#    8        return 0;
# Target 0: (a.out) stopped.
# (lldb) register read rip
#      rip = 0x0000000100000f4f  a.out`main + 15 at simpleprog.c:5:12
# (lldb) quit
# Quitting LLDB will kill one or more processes. Do you really want to proceed: [Y/n] y 