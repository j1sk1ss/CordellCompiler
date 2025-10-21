{
    function foo(i32 a = 10) => i32 {
        return 0;
    }

    function bar(i32 a = foo()) => i64 {
        return a;
    }

    start(i64 argc, ptr u64 argv) {
        exit bar();
    }
}