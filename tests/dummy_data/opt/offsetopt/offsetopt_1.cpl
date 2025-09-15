{
    function foo(i16 a) {
        : 24 : i8 b = 0;
        : 24 : i8 c = 0;
        return a;
    }

    start() {
        : 16 : i32 a = 0;
        : 24 : i32 b = 10;

        {
            : 32 : i16 c = 10; 
            : 32 : i16 d = 20;
        }

        : 24 : i32 d = a + 10;
        d = d + 1;

        foo(d);

        : 16 : i32 k = 10;
        : 24 : i32 f = 10;
        exit k;
    }
}