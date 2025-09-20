{
    function strlen(ptr i8 s) {
        i32 l = 0;
        while dref s; {
            s += 1;
            l += 1;
        }

        return l;
    }

    function puts(ptr i8 s) {
        return syscall(1, 1, s, strlen(s));
    }

    start() {
        puts("Hello there!");
        exit 0;
    }
}