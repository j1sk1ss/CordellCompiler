{
    function foo(i32 a, i32 b);
    start() {
        foo(1, 2);
    }
}

: OUTPUT
BB{X}: start
use $1;
use $2;
%4 = $1;
%2 <<= %4;
%5 = $2;
%3 <<= %5;
call foo0(i32 a, i32 b);
send
: