{
    function strlen(ptr i8 s) => i64 {
        i64 l = 0;
        while dref s; {
            s += 1;
            l += 1;
        }

        return l;
    }

    function foo() => i32 {
        return 12;
    }

    start(i64 argc, ptr u64 argv) {
        str msg = "Hello world!";
        syscall(0x2000004, 1, ref msg, strlen(ref msg));
        i8 a = foo();
        exit 0;
    }
}