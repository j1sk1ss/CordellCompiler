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

:/ OUTPUT
BB2: fn foo0(i32 a)
rbx = rdi;
rcx = rbx;
rax = rcx;
rax = rax + rbx;
rcx = rax;
rdx = $10;
rax = rcx;
rax = rax + rdx;
rcx = rax;
rdx = rcx;
rax = rbx;
rax = rax + rdx;
rcx = rax;
rax = rcx;
return rax;
BB3: fend
BB4: start
rcx = $1;
rcx = rcx;
use rcx;
push(rdi);
rdi <<= rcx;
push(rdx);
push(rcx);
call foo0(i32 a);
rcx = pop();
rdx = pop();
rdi = pop();
rdx = $10;
rax = rcx;
rax = rax + rdx;
rcx = rax;
rcx = rcx;
rcx = rcx;
rdi = rcx;
exit rdi;
BB5: send
/: