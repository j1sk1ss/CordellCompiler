function foo(i32 a, i32 b) {
    return a + b;
}

start() {
    foo(1, 2);
}

: OUTPUT
BB{X}: fn foo0(i32 a, i32 b)
r15 = rdi;
rcx = r15;
r15 = rsi;
rbx = r15;
r15 = rcx;
rax = r15;
rax = rax + rbx;
r15 = rax;
rdx = r15;
r15 = rdx;
rax = r15;
return rax;
BB{X}:
BB{X}: start
use $1;
use $2;
r15 = $1;
rdi <<= r15;
r15 = $2;
rsi <<= r15;
call foo0(i32 a, i32 b);
send
: