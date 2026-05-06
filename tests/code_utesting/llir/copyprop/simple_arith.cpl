start() {
    i32 a = 10;
    i32 b = 10;
    i32 c = a + b;
    i32 d = c + a;
    i32 e = d + c;
    exit e;
}

:/ OUTPUT
BB1: start
%7 = $10 + $10;
%8 = %7 + $10;
%9 = %8 + %7;
exit %9;
BB2: send
/: