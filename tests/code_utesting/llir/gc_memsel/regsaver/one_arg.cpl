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
rdx = rdi;
rbx = $10;
rax = rdx;
rax = rax + rbx;
rcx = rax;
rax = rcx;
return rax;
BB3:
BB4: start
rdx = $1;
rdx = rdx;
use rdx;
push(rdi);
rdi <<= rdx;
push(rbx);
push(rdx);
push(rcx);
call foo0(i32 a);
rcx = pop();
rdx = pop();
rbx = pop();
rdi = pop();
rbx = $1;
rax = rdx;
rax = rax + rbx;
rcx = rax;
rcx = rcx;
rcx = rcx;
rdi = rcx;
exit rdi;
BB5:
/: