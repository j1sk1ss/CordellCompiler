{
    start(i64 argc, ptr u64 argv) {
        i32 a = 0;
        asm(a) {
            "mov rax, &a",
            "xor rax, rax",
            "jmp .label",
            "mov rax, 0x60",
            "syscall"
        }

        exit 0;
    }
}