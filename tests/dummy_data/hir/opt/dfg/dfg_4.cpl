{
    function foo1() => i32 { return 1; }
    function foo2() => i32 { return 2; }

    start(i64 argc, ptr u64 argv) {
        i32 a = 10;
        if argc == 1; { a = foo1(); }
        else { a = foo2(); }
        exit a;
    }
}