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
                goto lb11;
                fn bar1() -> i0
                {
                    {
                        return num? 1;
                    }
                }
                lb11:
                u64t* %1 = &(bar1() -> i0);
                return u64t* %1;
            }
            lb21:
            {
                goto lb23;
                fn baz2() -> i0
                {
                    {
                        return num? 2;
                    }
                }
                lb23:
                u64t* %2 = &(baz2() -> i0);
                return u64t* %2;
            }
            goto lb8;
            lb7:
            i32t %4 = cnst? 1 as i32;
            i8t %3 = i32s %0 < i32t %4;
            if i8t %3, goto lb34, else goto lb35;
            lb34:
            goto lb21;
            lb35:
            i32t %6 = cnst? 1 as i32;
            i8t %5 = i32s %0 > i32t %6;
            if i8t %5, goto lb36, else goto lb37;
            lb36:
            goto lb21;
            lb37:
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