{
    function foo(ptr i8 a) {
        dref a = 23;
        return;
    } : -> void :

    start(i64 argc, ptr i64 argv) {
        i8 a = 0;
        foo(ref a);
        exit a;
    }
}