{
    @[entry]
    function foo(i32 a) {
        if a > 10; {
            return;
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
            lb26:
            i64t %2 = i32s %0 as i64;
            i64t %1 = i64t %2 > num? 10;
            if i64t %1, goto lb7, else goto lb9;
            lb7:
            {
                return ;
            }
            goto lb9;
            lb9:
            i64t %4 = i32s %0 as i64;
            i64t %3 = i64t %4 + num? 1;
            use i64t %3;
            i32s %0 = i64t %3;
            goto lb26;
            [unused] call _main0(i32 a), argc args(i64t %3,);
        }
    }
}
: