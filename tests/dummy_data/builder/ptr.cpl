{
    function foo(ptr int a) {
        dref a = 100;
        return;
    } : -> void :

    start(long argc, ptr long argv) {
        int a = 0;
        foo(ref a);

        arr array[10, int] =;
        foo(ref array[0]);
        exit a;
    }
}