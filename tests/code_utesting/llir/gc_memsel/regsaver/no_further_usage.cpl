function foo() {
    i32 a = 1;
    return a + 10;
}

start() {
    i32 a = 1;
    i32 b = a + 1;
    foo();
}

: OUTPUT
BB2: fn foo0()
r15 = $1;
rdx = r15;
r15 = rdx;
rcx = r15;
rbx movsxd rcx;
r15 = rbx;
rax = r15;
rax = rax + $10;
r15 = rax;
rsi = r15;
r15 = rsi;
rax = r15;
return rax;
BB3:
BB4: start
r15 = $1;
rsi = r15;
r15 = rsi;
rdi = r15;
rdx movsxd rdi;
r15 = rdx;
rax = r15;
rax = rax + $1;
r15 = rax;
rcx = r15;
r15 = rcx;
r8 = r15;
r15 = r8;
rbx = r15;
call foo0();
send
: