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
r15 = $1;
rdx = r15;
r15 = rdx;
rcx = r15;
r15 = $10;
rdi = r15;
r15 = rcx;
rax = r15;
rax = rax + rdi;
r15 = rax;
rsi = r15;
r15 = rsi;
rdx = r15;
exit rdx;
BB3:
BB4: start
r15 = $1;
rsi = r15;
r15 = rsi;
r8 = r15;
push(rcx);
push(rdx);
push(rax);
push(rdi);
push(r15);
call foo0();
r15 = pop();
rdi = pop();
rax = pop();
rdx = pop();
rcx = pop();
r15 = $1;
rcx = r15;
r15 = rcx;
rdi = r15;
r15 = r8;
rax = r15;
rax = rax + rdi;
r15 = rax;
r9 = r15;
r15 = r9;
rdx = r15;
r15 = rdx;
rdx = r15;
exit rdx;
BB5:
/:
