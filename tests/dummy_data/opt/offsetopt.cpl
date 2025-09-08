{
    function foo(i16 a) {
        i8 b = 0;
        i8 c = 0;
        return a;
    }

    start() {
        i32 a = 0; : 8 :
        i32 b = 10; : 16 :

        {
            i16 c = 10; : 24 : 
            i16 d = 20; : 24 :
        }

        i32 d = a + 10; : 16 :
        d = d + 1;

        foo(d);

        i32 k = 10; : 8  :
        i32 f = 10; : 16 :
        exit k;
    }
}