{
    function foo(i32 a, i32 b) {
    }
    start() {
        i32 a = 1;
        foo(1, 1);
        i32 b = a + 1;
        exit b;
    }
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
r12 = r15;
r15 = r12;
rdx = r15;
use $1;
use $1;
r15 = $1;
rdi <<= r15;
r15 = $1;
rsi <<= r15;
push(rax);
push(r15);
call foo0(i32 a, i32 b);
r15 = pop();
rax = pop();
r10 movsxd rdx;
r15 = r10;
rax = r15;
rax = rax + $1;
r15 = rax;
r9 = r15;
r15 = r9;
r8 = r15;
r15 = r8;
r11 = r15;
r15 = r11;
rdx = r15;
exit rdx;
BB4:
: