{
    function strlen(ptr i8 s) => i32 {
        i64 l = 0;
        while dref s; {
            s = s + 1;
            l = l + 1;
        }

        return l;
    } 

    function puts(ptr i8 s) => i32 {
        return syscall(1, 1, s, strlen(s));
    }

    start(i64 argc, ptr i64 argv) {
        puts(argv[1]);
        exit 0;
    }
}