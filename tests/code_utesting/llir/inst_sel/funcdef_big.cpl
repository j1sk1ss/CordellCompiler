function foo(...) {
    @[poparg] i32 a;
    @[poparg] i32 a1;
    @[poparg] i32 a2;
    @[poparg] i32 a3;
    @[poparg] i32 a4;
    @[poparg] i32 a5;
    @[poparg] i32 a6;
    @[poparg] i32 a7;
    @[poparg] i32 a8;
}

start() {
    foo(1);
}

:/ OUTPUT
BB2: fn foo0(...)
%0 = %9;
%1 = %10;
%2 = %11;
%3 = %12;
%4 = %13;
%5 = %14;
%6 = [rbp + 8];
%7 = [rbp + 16];
%8 = [rbp + 24];
fend
BB3: start
%15 <<= $1;
call foo0(...);
send
/: