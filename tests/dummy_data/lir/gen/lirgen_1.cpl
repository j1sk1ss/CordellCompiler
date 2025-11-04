{
    function strlen(ptr i8 s) => i64 {
        i64 l = 0;
        while dref s; {
            s += 1;
            l += 1;
        }

        return l;
    }

    start(i64 argc, ptr u64 argv) {
        str msg = "Hello world!";
        arr a[16, i8] = { 0, 1 };
        exit strlen(ref a);
    }
}