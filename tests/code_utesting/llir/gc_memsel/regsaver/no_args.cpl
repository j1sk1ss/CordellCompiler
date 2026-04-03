function foo() {
    i32 a = 1 as i32;
    exit a + 10 as i32;
}

start() {
    i32 a = 1 as i32;
    foo();
    i32 b = 1 as i32;
    exit (a + b) as i8;
}

:/ OUTPUT
BB2: fn foo0()
rsi = $1;
rdx = rsi;
rdi = $10;
rax = rdx;
rax = rax + rdi;
rcx = rax;
rdx = rcx;
exit rdx;
BB3:
BB4: start
r8 = $1;
rdx = r8;
push(rsi);
push(rdx);
push(rax);
push(rdi);
push(rcx);
call foo0();
rcx = pop();
rdi = pop();
rax = pop();
rdx = pop();
rsi = pop();
rsi = $1;
rcx = rsi;
rax = rdx;
rax = rax + rcx;
r9 = rax;
rdi = r9;
rdx = rdi;
exit rdx;
BB5:
/: