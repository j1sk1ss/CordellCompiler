{
    function foo(ptr long a) {
        dref a = 23;
        return;
    } : -> void :

    start(long argc, ptr long argv) {
        long a = 0;
        foo(ref a);
        exit a;
    }
}