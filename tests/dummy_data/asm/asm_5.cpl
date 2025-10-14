{
    function strlen(ptr i8 s) => i64 {
        i64 l = 0;
        while dref s; {
            s += 1;
            l += 1;
        }

        return l;
    }

    function puts(ptr i8 s) => i64 {
        return syscall(0x2000004, 1, s, strlen(s));
    }

    start(i64 argc, ptr u64 argv) {
        str hello = "Hello world!\n";
        puts(ref hello);
        exit 0;
    }
}