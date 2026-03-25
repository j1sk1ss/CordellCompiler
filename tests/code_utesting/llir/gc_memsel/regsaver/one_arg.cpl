function foo(i32 a) {
    return a + 10;
}

start() {
    i32 a = 1;
    foo(a);
    i32 b = a + 1;
    exit b;
}

: OUTPUT
BB2: fn foo0(i32 a)
r15 = rdi;
rbx = r15;
rdx movsxd rbx;
r15 = rdx;
rax = r15;
rax = rax + $10;
r15 = rax;
rcx = r15;
r15 = rcx;
rax = r15;
return rax;
BB3:
BB4: start
r15 = $1;
r9 = r15;
r15 = r9;
rcx = r15;
use rcx;
r15 = rcx;
rdi <<= r15;
push(r15);
push(rdx);
push(rax);
push(rcx);
call foo0(i32 a);
rcx = pop();
rax = pop();
rdx = pop();
r15 = pop();
rdx movsxd rcx;
r15 = rdx;
rax = r15;
rax = rax + $1;
r15 = rax;
r8 = r15;
r15 = r8;
rsi = r15;
r15 = rsi;
r10 = r15;
r15 = r10;
rdx = r15;
exit rdx;
BB5:
: