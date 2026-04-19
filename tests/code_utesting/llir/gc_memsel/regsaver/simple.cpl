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
rsi = rdi;
rdx = rsi;
rax = rdx;
rax = rax + rsi;
rdx = rax;
rbx = $10;
rax = rdx;
rax = rax + rbx;
rdx = rax;
rdx = rdx;
rax = rsi;
rax = rax + rdx;
rcx = rax;
rax = rcx;
return rax;
BB3:
BB4: start
rdx = $1;
rbx = rdx;
use rbx;
push(rdi);
rdi <<= rbx;
push(rbx);
push(rdx);
push(rcx);
call foo0(i32 a);
rcx = pop();
rdx = pop();
rbx = pop();
rdi = pop();
rdx = $10;
rax = rbx;
rax = rax + rdx;
rcx = rax;
rcx = rcx;
rcx = rcx;
rdi = rcx;
exit rdi;
BB5:
/: