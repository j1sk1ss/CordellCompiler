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
rax = rdi;
rax = rsi;
fend
BB3: start
r12 = $1;
r9 = r12;
rdx = $1;
use rdx;
r10 = $1;
use r10;
rdi <<= rdx;
rsi <<= r10;
push(rax);
call foo0(i32 a, i32 b);
rax = pop();
rcx = $1;
rax = r9;
rax = rax + rcx;
r8 = rax;
r11 = r8;
r13 = r11;
rdx = r13;
exit rdx;
BB4:
/: