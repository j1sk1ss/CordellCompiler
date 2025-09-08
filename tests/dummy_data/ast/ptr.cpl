{
    function foo(ptr i32 a) {
        dref a = 100;
        return;
    } : -> void :

    start(i64 argc, ptr i64 argv) {
        i32 a = 0;
        foo(ref a);

        arr array[10, i32] =;
        foo(ref array[0]);
        exit a;
    }
}