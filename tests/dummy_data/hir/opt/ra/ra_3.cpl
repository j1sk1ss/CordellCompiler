{
    function foo(u32 a, f64 b) => f64 {
        return b;
    }

    start(i64 argc, ptr u64 argv) {
        f64 a = 10;
        i32 b = 10;
        f64 d = a + b;
        b += a;

        b = a;

        i32 c = foo(a, b);
        exit c;
    }
}