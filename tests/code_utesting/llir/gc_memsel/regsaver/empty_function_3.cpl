function foo(i32 a, i32 b) {
}
start() {
    i32 a = 1 as i32;
    foo(1 as i32, 1 as i32);
    i32 b = a + 1 as i32;
    exit b as i8;
}

:/ OUTPUT
BB2: fn foo0(i32 a, i32 b)
rcx = rdi;
rcx = rsi;
fend
BB3: start
rcx = $1;
rbx = rcx;
rcx = $1;
use rcx;
rdx = $1;
use rdx;
push(rdi);
rdi <<= rcx;
push(rsi);
rsi <<= rdx;
push(rcx);
call foo0(i32 a, i32 b);
rcx = pop();
rsi = pop();
rdi = pop();
rdx = $1;
rax = rbx;
rax = rax + rdx;
rcx = rax;
rcx = rcx;
rcx = rcx;
rdi = rcx;
exit rdi;
BB4: send
/: