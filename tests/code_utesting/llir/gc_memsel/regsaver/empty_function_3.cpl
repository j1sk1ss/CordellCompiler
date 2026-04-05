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
r13 = $1;
r10 = r13;
rbx = $1;
use rbx;
r11 = $1;
use r11;
rdi <<= rbx;
rsi <<= r11;
call foo0(i32 a, i32 b);
rdx = $1;
rax = r10;
rax = rax + rdx;
r9 = rax;
r12 = r9;
r14 = r12;
rdx = r14;
exit rdx;
BB4:
/: