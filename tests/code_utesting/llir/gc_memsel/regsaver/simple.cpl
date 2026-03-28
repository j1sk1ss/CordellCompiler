function foo(i32 a) {
    i32 b = a;
    i32 c = b + a + 10 as i32;
    return a + c;
}

start() {
    i32 a = 1 as i32;
    foo(a);
    i32 b = a + 10 as i32;
    exit b as i8;
}

: OUTPUT
BB2: fn foo0(i32 a)
r15 = rdi;
rdx = r15;
r15 = rdx;
r10 = r15;
r15 = r10;
rax = r15;
rax = rax + rdx;
r15 = rax;
rsi = r15;
r15 = $10;
r9 = r15;
r15 = rsi;
rax = r15;
rax = rax + r9;
r15 = rax;
rcx = r15;
r15 = rcx;
rbx = r15;
r15 = rdx;
rax = r15;
rax = rax + rbx;
r15 = rax;
r8 = r15;
r15 = r8;
rax = r15;
return rax;
BB3:
BB4: start
r15 = $1;
rsi = r15;
r15 = rsi;
r8 = r15;
use r8;
r15 = r8;
rdi <<= r15;
push(r8);
push(rcx);
push(r9);
push(r10);
push(rax);
push(rdx);
push(r15);
call foo0(i32 a);
r15 = pop();
rdx = pop();
rax = pop();
r10 = pop();
r9 = pop();
rcx = pop();
r8 = pop();
r15 = $10;
rcx = r15;
r15 = r8;
rax = r15;
rax = rax + rcx;
r15 = rax;
r9 = r15;
r15 = r9;
r10 = r15;
r15 = r10;
rdx = r15;
r15 = rdx;
rdx = r15;
exit rdx;
BB5:
: