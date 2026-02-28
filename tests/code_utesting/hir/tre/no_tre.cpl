{
    @[entry]
    function foo(i32 a) {
        if a > 10; {
            foo(a + 1);
        }

        foo(a + 1);
    }
}

: OUTPUT
{
    fn _main0(i32 a)
    {
        i32s %0 = alloc(8);
        i32s %0 = load_starg();
        {
            lb33:
            i64t %2 = i32s %0 as i64;
            i64t %1 = i64t %2 > num? 10;
            if i64t %1, goto lb7, else goto lb9;
            lb7:
            {
                i64t %4 = i32s %0 as i64;
                i64t %3 = i64t %4 + num? 1;
                use i64t %3;
                call _main0(i32 a), argc args(i64t %3,);
            }
            goto lb9;
            lb9:
            i64t %6 = i32s %0 as i64;
            i64t %5 = i64t %6 + num? 1;
            use i64t %5;
            i32s %0 = i64t %5;
            goto lb33;
            [unused] call _main0(i32 a), argc args(i64t %5,);
        }
    }
}
: