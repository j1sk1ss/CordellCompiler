function foo(i32 a, i32 b) {
}
start() {
    foo(1, 2);
}

:/ OUTPUT
BB2: fn foo0(i32 a, i32 b)
%0 = %2;
%1 = %3;
fend
BB3: start
%4 <<= $1;
%5 <<= $2;
call foo0(i32 a, i32 b);
send
/: