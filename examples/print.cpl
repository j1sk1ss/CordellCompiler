{
    function strlen(ptr i8 s) => i64 {
        i64 l = 0;
        while dref s; {
            s += 1;
            l += 1;
        }

        return l;
    }

    function print(ptr str msg) => i32 {
        return syscall(0x2000004, 1, msg, strlen(msg));
    }

    start(i64 argc, ptr u64 argv) {
        print("Hello world!\n");
        exit 0;
    }
}