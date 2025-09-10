{
    start(i64 argc, ptr u64 argv) {
        i32 a  = 10
        i32 b  = 30;
        i32 b1 = 50;
        asm(a, b) {
            "mov rax, &a",
            "mov &b, rax"
        }

        i32 c = 40;
        exit c;
    }
}