{
    function foo(short a) {
        char b = 0;
        char c = 0;
        return a;
    }

    start {
        int a = 0; : 8 :
        int b = 10; : 16 :

        {
            short c = 10; : 24 : 
            short d = 20; : 24 :
        }

        int d = a + 10; : 16 :
        d = d + 1;

        foo(d);

        int k = 10; : 8  :
        int f = 10; : 16 :
        exit k;
    } exit 0;
}