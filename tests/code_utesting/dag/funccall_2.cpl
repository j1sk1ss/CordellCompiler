{
    function foo(i32 a, i32 b) -> i32;
    start() {
        foo(100 + 1, 100 + 1);
    }
}

: OUTPUT
{
    start {
        {
            {
                i64t %2 = num? 100 + num? 1;
                use i64t %2;
                [unused] i64t %3 = num? 100 + num? 1;
                use i64t %2;
                call foo0(i32 a, i32 b) -> i32, argc args(i64t %2,i64t %2,);
            }
        }
    }
}
: