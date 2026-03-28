function foo() {
    i32 a = 1 as i32;
    return a + 10 as i32;
}

start() {
    i32 a = 1 as i32;
    i32 b = a + 1 as i32;
    foo();
}

: OUTPUT
BB2: fn foo0()
r15 = $1;
rcx = r15;
r15 = rcx;
rsi = r15;
r15 = $10;
rbx = r15;
r15 = rsi;
rax = r15;
rax = rax + rbx;
r15 = rax;
rdx = r15;
r15 = rdx;
rax = r15;
return rax;
BB3:
BB4: start
r15 = $1;
rsi = r15;
r15 = rsi;
rcx = r15;
r15 = $1;
rdx = r15;
r15 = rcx;
rax = r15;
rax = rax + rdx;
r15 = rax;
rbx = r15;
r15 = rbx;
rdi = r15;
call foo0();
send
: