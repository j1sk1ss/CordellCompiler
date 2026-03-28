function foo(...);
start() {
    i32 a = 1 as i32;
    i32 b = 2 as i32;
    i32 c = 3 as i32;
    foo(a, 1, 2, b, 3, 4, c, 5, 6);
}

: OUTPUT
BB1: start
r15 = $1;
rcx = r15;
r15 = rcx;
r12 = r15;
r15 = $2;
r13 = r15;
r15 = r13;
rax = r15;
r15 = $3;
r8 = r15;
r15 = r8;
r9 = r15;
use r12;
use rax;
use r9;
r15 = r12;
rdi <<= r15;
r15 = $1;
rsi <<= r15;
r15 = $2;
rdx <<= r15;
r15 = rax;
rcx <<= r15;
r15 = $3;
r8 <<= r15;
r15 = $4;
r9 <<= r15;
push(r9);
push($5);
push($6);
call foo0(...);
send
: