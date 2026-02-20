{
    function foo() -> i0;
    function foo(i32 a) -> i0;
    start() {
        foo();
        foo(10 as i32);
        exit 0;
    }
}

: OUTPUT
{
    start {
        {
            {
                call foo0() -> i0, argc args();
                i32t %1 = num? 10 as i32;
                use i32t %1;
                call foo1(i32 a) -> i0, argc args(i32t %1,);
                exit num? 0;
            }
        }
    }
}
: