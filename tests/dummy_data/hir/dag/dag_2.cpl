{
    function foo(ptr i32 a) => i32 {
        dref a = 1;
        return 0;
    }
    start() {
        i32 a = 10;
        i32 b = 10;
        ptr i32 c = ref a;
        dref c = 11;
        i32 d = a + b;
        foo(ref a);
        exit d;
    }
}