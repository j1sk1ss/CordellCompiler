: BUG :
start() {
    i32 a = 10 as i32;
    i32 b = 10 as i32;
    i32 c = a + b;
    i32 d = a - b;
}

: OUTPUT
BB1: start
%5 = $10 as i32;
%1 = %5;
%4 = $10 as i32;
%0 = %4;
%6 = %0 + %1;
%2 = %6;
%7 = %0 - %1;
%3 = %7;
send
: