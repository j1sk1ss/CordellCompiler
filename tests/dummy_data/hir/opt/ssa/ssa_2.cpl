{
    function foo(i32 b = 10) => i32 {
        if b == 10; {
            return 0;
        }

        b = 1;
        exit b;
    }

    start(i64 argc, ptr u64 argv) {
        i32 b = 10;
        foo();
        exit b;
    }
}