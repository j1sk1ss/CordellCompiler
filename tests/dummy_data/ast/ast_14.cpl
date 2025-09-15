{
    function strlen(ptr i8 s) => i64 {
        i64 l = 0;
        while dref s; {
            s = s + 1;
            l = l + 1;
        }

        return l;
    }

    function puts(ptr i8 s) {
        i64 l = strlen(s);
        asm (s, l) {
            "mov rax, 1",
            "mov rdi, 1",
            "mov rsi, &s",
            "mov rdx, &l",
            "syscall"
        }
        return;
    }

    start(i64 argc, ptr u64 argv) {
        puts("Hello world!");
        exit 0;
    }
}