start() {
    i32 a = (10 + 10) as i32;
    function foo(i32 a) {
    }
    foo(a);
    foo((10 + 10) as i32);
}

:/ OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i64t %3 = num? 10 + num? 10;
                i32t %2 = i64t %3 as i32;
                i32s %6 = i32t %2;
                goto lb{X}
                fn foo1(i32 a)
                {
                    i32s %1 = alloc(8);
                    i32s %7 = load_arg();
                    {
                    }
                }
                lb{X}
                use i32s %6;
                call foo1(i32 a), argc args(i32s %6,);
                [unused] i64t %5 = num? 10 + num? 10;
                i32t %4 = i64t %3 as i32;
                use i32t %4;
                call foo1(i32 a), argc args(i32t %4,);
            }
        }
    }
}
/:
