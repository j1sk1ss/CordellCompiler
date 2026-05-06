function foo(i32 a, i32 b) {
    return a + b;
}

start() {
    foo(1, 2);
}

:/ OUTPUT
BB2: fn foo0(i32 a, i32 b)
rcx = rdi;
rdx = rsi;
rax = rcx;
rax = rax + rdx;
rcx = rax;
rax = rcx;
return rax;
BB3: fend
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