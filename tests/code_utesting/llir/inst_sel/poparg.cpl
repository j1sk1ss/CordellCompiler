function foo(...) -> i0 {
    @[poparg] i32 a;
    @[poparg] i32 b;
}

start() {
    foo(1, 2);
}

:/ OUTPUT
BB2: fn foo0(...) -> i0
%2 = &([rbp + 8]);
%0 = *(%2);
%5 = %2;
%6 = %5 + 4;
%3 = %6;
%2 = %3;
%1 = *(%2);
%7 = %2;
%8 = %7 + 4;
%4 = %8;
%2 = %4;
fend
BB3: start
push(rsi);
%9 <<= $2;
push(rdi);
%10 <<= $1;
rax = rax ^ rax;
call foo0(...) -> i0;
rdi = pop();
rsi = pop();
send
/: