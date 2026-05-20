function foo(...);
start() {
    foo(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
}

:/ OUTPUT
BB1: start
push($11);
push($10);
push($9);
push($8);
push($7);
push(r9);
%0 <<= $6;
push(r8);
%1 <<= $5;
push(rcx);
%2 <<= $4;
push(rdx);
%3 <<= $3;
push(rsi);
%4 <<= $2;
push(rdi);
%5 <<= $1;
call foo0(...);
rdi = pop();
rsi = pop();
rdx = pop();
rcx = pop();
r8 = pop();
r9 = pop();
send
/: