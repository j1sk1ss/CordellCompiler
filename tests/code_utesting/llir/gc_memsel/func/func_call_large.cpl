function foo(...);
start() {
    i32 a = 1 as i32;
    i32 b = 2 as i32;
    i32 c = 3 as i32;
    foo(a, 1, 2, b, 3, 4, c, 5, 6);
}

:/ OUTPUT
BB1: start
r8 = $1;
r13 = r8;
r9 = $2;
rcx = r9;
rax = $3;
r12 = rax;
use r13;
use rcx;
use r12;
rdi <<= r13;
rsi <<= $1;
rdx <<= $2;
rcx <<= rcx;
r8 <<= $3;
r9 <<= $4;
push(r12);
push($5);
push($6);
call foo0(...);
send
/: