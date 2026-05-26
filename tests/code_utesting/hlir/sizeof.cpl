container complex {
    i32 a;
    i8  b;
    i16 c;
}

@[align(1)]
container simple {
    i8  a;
    i16 b;
}

start() {
    i32 a = sizeof(complex);
    i32 b = sizeof(simple);
    i32 c = sizeof(f64);
    i32 d = sizeof(i8);
}

:/ OUTPUT
BB1: start
%9 = 24 as i32;
%5 = %9;
%10 = 3 as i32;
%6 = %10;
%11 = 8 as i32;
%7 = %11;
%12 = 1 as i32;
%8 = %12;
send
/: