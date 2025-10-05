{
    function foo(i32 x, i32 y) => i32 {
        return x + y;
    }

    start(i64 argc, ptr u64 argv) {
        i32 a = 1;   : stack :
        i32 b = 2;   : stack :
        i32 c = 3;   : stack :
        i32 d = 4;   : reg :
        i32 e = 5;   : reg :
        i32 f = 6;   : reg :
        i32 g = 7;   : reg :
        i32 h = 8;   : reg :
        i32 i = 9;   : reg :
        i32 j = 10;  : reg :

        ptr i32 pa = ref a;  : to the end :
        ptr i32 pb = ref b;
        ptr i32 pc = ref c;

        i32 sum1 = foo(a, b);  : to if :
        i32 sum2 = foo(c, d);  : to if :
        i32 sum3 = foo(e, f);  : dies immidiatly :
        i32 sum4 = foo(g, h);  : dies immidiatly :

        i32 x = dref pa;       : to if :
        i32 y = dref pb;       : to if :
        i32 z = dref pc;       : to if and one more block :

        if (sum1 + sum2) > (x + y); {
            j = j + z;         : sum1, sum2, x, y should die :
        } 
        else {
            j = j - z;         : z still alive :
        }

        : still alive j and z :

        i64 big1 = argc * 111; : lives till exit :
        i64 big2 = argc * 222; : lives till exit :
        i64 big3 = argc * 333; : lives till exit :
        i64 big4 = argc * 444; : lives till exit :
        i64 big5 = argc * 555; : lives till exit :
        i64 big6 = argc * 666; : lives till exit :

        exit big1 + big2 + big3 + big4 + big5 + big6 + j;
    }
}
