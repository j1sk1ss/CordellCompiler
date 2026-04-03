function foo() {
    i32 a = 1 as i32;
    return a + 10 as i32;
}

start() {
    i32 a = 1 as i32;
    i32 b = a + 1 as i32;
    foo();
}

:/ OUTPUT
BB2: fn foo0()
rdx = $1;
rsi = rdx;
rbx = $10;
rax = rsi;
rax = rax + rbx;
rcx = rax;
rax = rcx;
return rax;
BB3:
BB4: start
rdi = $1;
rcx = rdi;
rbx = $1;
rax = rcx;
rax = rax + rbx;
rsi = rax;
rdx = rsi;
call foo0();
send
/: