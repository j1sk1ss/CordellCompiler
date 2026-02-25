{
    function factory(i32 k) -> ptr u64 {
        switch k; {
            case 1; {
                function bar() -> i0 {
                    return 1;
                }
                return bar;
            }
            default {
                function baz() -> i0 {
                    return 2;
                }
                return baz;
            }
        }
    }

    start() {
        exit factory(0)();
    }
}

: OUTPUT
{
    fn factory0(i32 k) -> u64*
    {
        i32s %0 = alloc(8);
        i32s %0 = load_arg();
        {
            goto lb7;
            lb9:
            {
                fn bar1() -> i0
                {
                    {
                        return num? 1;
                    }
                }
                u64t* %1 = &(bar1() -> i0);
                return u64t* %1;
            }
            lb20:
            {
                fn baz2() -> i0
                {
                    {
                        return num? 2;
                    }
                }
                u64t* %2 = &(baz2() -> i0);
                return u64t* %2;
            }
            goto lb8;
            lb7:
            i32t %4 = cnst? 1 as i32;
            i8t %3 = i32s %0 < i32t %4;
            if i8t %3, goto lb32, else goto lb33;
            lb32:
            goto lb20;
            lb33:
            i32t %6 = cnst? 1 as i32;
            i8t %5 = i32s %0 > i32t %6;
            if i8t %5, goto lb34, else goto lb35;
            lb34:
            goto lb20;
            lb35:
            goto lb9;
            lb8:
        }
    }
    start {
        {
            {
                use num? 0;
                u64t* %7 = call factory0(i32 k) -> u64*, argc args(num? 0,);
                i64t %8 = call u64t* %7, argc args();
                exit i64t %8;
            }
        }
    }
}
: