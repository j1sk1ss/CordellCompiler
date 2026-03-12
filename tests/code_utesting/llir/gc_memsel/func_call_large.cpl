function foo(...);
start() {
    i32 a = 1;
    i32 b = 2;
    i32 c = 3;
    foo(a, 1, 2, b, 3, 4, c, 5, 6);
}

: OUTPUT
BB1: start
r15 = $1;
r8 = r15;
r15 = r8;
r13 = r15;
r15 = $2;
r14 = r15;
r15 = r14;
rcx = r15;
r15 = $3;
r9 = r15;
r15 = r9;
r12 = r15;
use r13;
use $1;
use $2;
use rcx;
use $3;
use $4;
use r12;
use $5;
use $6;
r15 = r13;
rdi <<= r15;
r15 = $1;
rsi <<= r15;
r15 = $2;
rdx <<= r15;
r15 = rcx;
rcx <<= r15;
r15 = $3;
r8 <<= r15;
r15 = $4;
r9 <<= r15;
push(r12);
push($5);
push($6);
call foo0(...);
send
: