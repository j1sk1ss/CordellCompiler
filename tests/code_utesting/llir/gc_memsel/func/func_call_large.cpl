function foo(...);
start() {
    i32 a = 1 as i32;
    i32 b = 2 as i32;
    i32 c = 3 as i32;
    foo(a, 1, 2, b, 3, 4, c, 5, 6);
}

:/ OUTPUT
BB1: start
rcx = $1;
r9 = rcx;
rcx = $2;
rdx = rcx;
rcx = $3;
rcx = rcx;
use r9;
use rdx;
use rcx;
push(rdi);
rdi <<= r9;
push(rsi);
rsi <<= $1;
push(rdx);
rdx <<= $2;
push(rcx);
rcx <<= rdx;
push(r8);
r8 <<= $3;
push(r9);
r9 <<= $4;
push(rcx);
push($5);
push($6);
call foo0(...);
r9 = pop();
r8 = pop();
rcx = pop();
rdx = pop();
rsi = pop();
rdi = pop();
send
/: