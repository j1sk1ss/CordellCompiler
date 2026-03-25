{
    function foo(u32 a);
    start() {
        ptr i0 a = foo;
        a(10);
        foo(10);
    }
}

: OUTPUT
BB{X} start
%2 = &(foo0(u32 a));
%1 = %2;
stparam($10);
call %1;
stparam($10);
call foo0(u32 a);
send
: