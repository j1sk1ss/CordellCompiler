: BUG :
function foo() -> i0;
function bar() -> i0;
start() {
    foo();
    i32 a = 10 as i32;
    bar();
    i32 b = a + 10 as i32;
}

:/ OUTPUT
BB1: start
call foo0() -> i0;
call bar1() -> i0;
%2 = $10 as i32;
%0 = %2;
%3 = $10 as i32;
%4 = %0 + %3;
%1 = %4;
send
/:
