function foo() {
    i32 a = 1;
    exit a + 10;
}

start() {
    i32 a = 1;
    foo();
    i32 b = 1;
    exit a + b;
}

: OUTPUT
BB2: fn foo0()
r15 = $1;
rcx = r15;
r15 = rcx;
rdi = r15;
rdx movsxd rdi;
r15 = rdx;
rax = r15;
rax = rax + $10;
r15 = rax;
rsi = r15;
r15 = rsi;
rdx = r15;
exit rdx;
BB3:
BB4: start
r15 = $1;
rdi = r15;
r15 = rdi;
rcx = r15;
push(rcx);
push(rsi);
push(rdx);
push(rax);
push(r15);
call foo0();
r15 = pop();
rax = pop();
rdx = pop();
rsi = pop();
rcx = pop();
r15 = $1;
rdx = r15;
r15 = rdx;
r8 = r15;
r15 = rcx;
rax = r15;
rax = rax + r8;
r15 = rax;
rsi = r15;
r15 = rsi;
rdx = r15;
exit rdx;
BB5:
: