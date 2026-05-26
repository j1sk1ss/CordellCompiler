function foo(i32 a, i32 b) {
}
start() {
    foo(1, 2);
}

:/ OUTPUT
BB2: fn foo0(i32 a, i32 b)
[SSA] %0 = %2;
[SSA] %1 = %3;
fend
BB3: start
push(rsi);
%4 <<= $2;
push(rdi);
%5 <<= $1;
call foo0(i32 a, i32 b);
rdi = pop();
rsi = pop();
send
/: