{
    start(i64 argc, ptr u64 argv) {
        : 24 : i32 a  = 10;
        : 32 : i32 b  = 30;
        : 40 : i32 b1 = 50;
        asm(a, b) {
            "mov rax, &a",
            "mov &b, rax",
        }

        : 24 : i32 c = 40;
        exit c;
    }
}