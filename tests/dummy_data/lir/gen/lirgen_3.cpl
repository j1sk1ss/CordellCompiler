{
    start(i64 argc, ptr u64 argv) {
        i32 a = 10;
        i32 b;
        asm (ref a, b) {
            "mov rax, %0",
            "mov rax, [rax]",
            "mov %1, rax"
        }

        exit b;
    }
}