{
    function foo(u32 a);
    start() {
        ptr u32 a = foo;
        a(10);
        foo(10);
    }
}

: OUTPUT
BB{X}: start
%2 = &(foo0(u32 a));
%3 = %2 as u64;
%1 = %3;
use $10;
stparam($10);
call %1;
use $10;
stparam($10);
call foo0(u32 a);
send
: