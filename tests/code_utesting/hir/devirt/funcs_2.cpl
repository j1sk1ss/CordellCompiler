{
    function foo(i32 a) -> i0;
    function foo(i64 a) -> i0;
    start() {
        foo(10 as i64);
        foo(10 as i32);
        exit 0;
    }
}

: OUTPUT
{
    start {
        {
            {
                i64t %2 = num? 10 as i64;
                use i64t %2;
                call foo1(i64 a) -> i0, argc args(i64t %2,);
                i32t %3 = num? 10 as i32;
                use i32t %3;
                call foo0(i32 a) -> i0, argc args(i32t %3,);
                exit num? 0;
            }
        }
    }
}
: