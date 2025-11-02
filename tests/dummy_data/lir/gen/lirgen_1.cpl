{
    function strlen(ptr i8 s) => i64 {
        i64 l = 0;
        while dref s > 10; {
            s /= 1;
            l = l % 1;
        }

        return l;
    }

    start(i64 argc, ptr u64 argv) {
        str msg = "Hello world!";
        exit strlen(ref msg);
    }
}