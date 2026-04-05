function foo(i32 a, i32 b) {
    return a + b;
}

start() {
    foo(1, 2);
}

:/ OUTPUT
BB2: fn foo0(i32 a, i32 b)
rbx = rdi;
rsi = rsi;
rax = rbx;
rax = rax + rsi;
rdx = rax;
rax = rdx;
return rax;
BB3:
BB4: start
rdi <<= $1;
rsi <<= $2;
call foo0(i32 a, i32 b);
send
/: