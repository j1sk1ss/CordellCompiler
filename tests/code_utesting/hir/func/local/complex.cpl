{
    @[entry]
    function foo(i32 a) {
        i32 b;
        function bar(i32 a, i32 b) {
            a += 1;
            b += 1;
        }
        bar(b, a);
    }
}

: OUTPUT
{
    fn _main0(i32 a)
    {
        i32s %0 = alloc(8);
        i32s %0 = load_starg();
        {
            i32s %1 = alloc(8);
            goto lb8;
            fn bar1(i32 a, i32 b)
            {
                i32s %2 = alloc(8);
                i32s %2 = load_arg();
                i32s %3 = alloc(8);
                i32s %3 = load_arg();
                {
                    i32t %5 = num? 1 as i32;
                    i32t %4 = i32s %2 + i32t %5;
                    i32s %2 = i32t %4;
                    i32t %7 = num? 1 as i32;
                    i32t %6 = i32s %3 + i32t %7;
                    i32s %3 = i32t %6;
                }
            }
            lb8:
            use i32s %1;
            use i32s %0;
            call bar1(i32 a, i32 b), argc args(i32s %1,i32s %0,);
        }
    }
}
: