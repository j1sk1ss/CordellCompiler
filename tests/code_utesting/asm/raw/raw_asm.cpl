start() {
    asm(1) {
        "mov rax, %0",
        "syscall"
    }
}

:/ OUTPUT
section .text
global _main
_main{X}
push rbp
mov rbp, rsp
mov rax, 1
syscall
mov rax, 0x2000001
syscall
/:
