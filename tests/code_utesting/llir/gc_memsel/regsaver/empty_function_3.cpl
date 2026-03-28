function foo(i32 a, i32 b) {
}
start() {
    i32 a = 1 as i32;
    foo(1 as i32, 1 as i32);
    i32 b = a + 1 as i32;
    exit b as i8;
}

: OUTPUT
BB2: fn foo0(i32 a, i32 b)
r15 = rdi;
rax = r15;
r15 = rsi;
rax = r15;
fend
BB3: start
r15 = $1;
r13 = r15;
r15 = r13;
rdx = r15;
r15 = $1;
r10 = r15;
use r10;
r15 = $1;
r12 = r15;
use r12;
r15 = r10;
rdi <<= r15;
r15 = r12;
rsi <<= r15;
push(rax);
push(r15);
call foo0(i32 a, i32 b);
r15 = pop();
rax = pop();
r15 = $1;
rcx = r15;
r15 = rdx;
rax = r15;
rax = rax + rcx;
r15 = rax;
r11 = r15;
r15 = r11;
r8 = r15;
r15 = r8;
r9 = r15;
r15 = r9;
rdx = r15;
exit rdx;
BB4:
: