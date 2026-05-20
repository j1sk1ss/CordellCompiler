function foo(i32 a, i32 b);
start() {
    foo(1, 2);
}

:/ OUTPUT
BB1: start
push(rsi);
%2 <<= $2;
push(rdi);
%3 <<= $1;
call foo0(i32 a, i32 b);
rdi = pop();
rsi = pop();
send
/: