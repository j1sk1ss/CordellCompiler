{
    function foo(i16 a) {
        : 16 : i8 b = 0;
        : 16 : i8 c = 0;
        return a;
    }

    start() {
        : 8 : i32 a = 0;
        : 16 : i32 b = 10;

        {
            : 24 : i16 c = 10; 
            : 24 : i16 d = 20;
        }

        : 16 : i32 d = a + 10;
        d = d + 1;

        foo(d);

        : 8 : i32 k = 10;
        : 16 : i32 f = 10;
        exit k;
    }
}