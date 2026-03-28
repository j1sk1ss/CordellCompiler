function foo(i32 a) {
    return a + 10 as i32;
}

start() {
    i32 a = 1 as i32;
    foo(a);
    i32 b = a + 1 as i32;
    exit b as i8;
}

:/ OUTPUT
BB2: fn foo0(i32 a)
r15 = rdi;
rbx = r15;
r15 = $10;
rcx = r15;
r15 = rbx;
rax = r15;
rax = rax + rcx;
r15 = rax;
rdx = r15;
r15 = rdx;
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
push(rcx);
push(rdx);
push(rax);
push(r15);
call foo0(i32 a);
r15 = pop();
rax = pop();
rdx = pop();
rcx = pop();
r15 = $1;
r8 = r15;
r15 = rcx;
rax = r15;
rax = rax + r8;
r15 = rax;
rdx = r15;
r15 = rdx;
r10 = r15;
r15 = r10;
rsi = r15;
r15 = rsi;
rdx = r15;
exit rdx;
BB5:
/:
