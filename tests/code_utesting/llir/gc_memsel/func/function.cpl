function foo(i32 a, i32 b) {
    return a + b;
}

start() {
    foo(1, 2);
}

:/ OUTPUT
BB2: fn foo0(i32 a, i32 b)
rdx = rdi;
rbx = rsi;
rax = rdx;
rax = rax + rbx;
rcx = rax;
rax = rcx;
return rax;
BB3:
BB4: start
push(rdi);
rdi <<= $1;
push(rsi);
rsi <<= $2;
call foo0(i32 a, i32 b);
rsi = pop();
rdi = pop();
send
/: