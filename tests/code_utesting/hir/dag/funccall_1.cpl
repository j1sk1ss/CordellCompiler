{
    function foo(i32 a) -> i32;
    start() {
        foo((10 + 10) + (10 + 10));
    }
}

: OUTPUT
{
    start {
        {
            {
                i64t %1 = num? 10 + num? 10;
                [unused] i64t %2 = num? 10 + num? 10;
                i64t %3 = i64t %1 + i64t %1;
                use i64t %3;
                call foo0(i32 a) -> i32, argc args(i64t %3,);
            }
        }
    }
}
: