{
    function foo(i32 a, i32 b) {
    }
    start() {
        foo(1, 2);
    }
}

: OUTPUT
BB{X}: fn foo0(i32 a, i32 b)
%4 = %2;
%0 = %4;
%5 = %3;
%1 = %5;
fend
BB{X}: start
use $1;
use $2;
%8 = $1;
%6 <<= %8;
%9 = $2;
%7 <<= %9;
call foo0(i32 a, i32 b);
send
: