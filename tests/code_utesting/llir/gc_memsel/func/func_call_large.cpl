function foo(...);
start() {
    i32 a = 1 as i32;
    i32 b = 2 as i32;
    i32 c = 3 as i32;
    foo(a, 1, 2, b, 3, 4, c, 5, 6);
}

:/ OUTPUT
BB1: start
r9 = $1;
r14 = r9;
r10 = $2;
rdx = r10;
rcx = $3;
r13 = rcx;
use r14;
use rdx;
use r13;
rdi <<= r14;
rsi <<= $1;
rdx <<= $2;
rcx <<= rdx;
r8 <<= $3;
r9 <<= $4;
push(r13);
push($5);
push($6);
call foo0(...);
send
/: