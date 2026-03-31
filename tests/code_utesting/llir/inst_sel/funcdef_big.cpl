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
BB{X} fn foo0(...)
%15 = %9;
%0 = %15;
%16 = %10;
%1 = %16;
%17 = %11;
%2 = %17;
%18 = %12;
%3 = %18;
%19 = %13;
%4 = %19;
%20 = %14;
%5 = %20;
%21 = [rbp + 8];
%6 = %21;
%22 = [rbp + 16];
%7 = %22;
%23 = [rbp + 24];
%8 = %23;
fend
BB{X} start
%25 = $1;
%24 <<= %25;
call foo0(...);
send
/:
