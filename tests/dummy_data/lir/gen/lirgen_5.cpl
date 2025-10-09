{
    function foo(i32 a, i32 b) => i32 {
        i32 c = a + b;
        return a + b;
    }

    start(i64 argc, ptr u64 argv) {
        i32 a = 10;
        i32 b = 10;
        i32 c = a + b;
        a = 1;
        exit foo(a + b, a + b);
    }
}