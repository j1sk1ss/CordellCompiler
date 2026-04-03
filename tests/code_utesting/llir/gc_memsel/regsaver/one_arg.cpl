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
rbx = rdi;
rcx = $10;
rax = rbx;
rax = rax + rcx;
rdx = rax;
rax = rdx;
return rax;
BB3:
BB4: start
rsi = $1;
rcx = rsi;
use rcx;
rdi <<= rcx;
push(rdx);
push(rax);
push(rcx);
call foo0(i32 a);
rcx = pop();
rax = pop();
rdx = pop();
r9 = $1;
rax = rcx;
rax = rax + r9;
rdx = rax;
r10 = rdx;
r8 = r10;
rdx = r8;
exit rdx;
BB5:
/: