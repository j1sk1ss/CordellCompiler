{
    start(i64 argc, ptr u64 argv) {
        i32 a = 10;
        i32 b;
        asm (a, b) {
            "mov rax, {a}",
            "mov {b}, rax"
        }

        exit b;
    }
}