start() {
    i32 a = 10 + 10;
    function foo(i32 a) {
    }
    foo(a);
    foo(10 + 10);
}

: OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i64t %2 = num? 10 + num? 10;
                i32t %3 = i64t %2 as i32;
                i32s %5 = i32t %3;
                fn foo0(i32 a)
                {
                    i32s %6 = alloc(8);
                    i32s %6 = load_arg();
                    {
                    }
                }
                use i32s %5;
                call foo0(i32 a), argc args(i32s %5,);
                [unused] i64t %4 = num? 10 + num? 10;
                use i64t %2;
                call foo0(i32 a), argc args(i64t %2,);
            }
        }
    }
}
: