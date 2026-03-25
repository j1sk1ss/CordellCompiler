function factory(i32 k) -> ptr i0 {
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

: OUTPUT
{
    fn factory0(i32 k) -> i0*
    {
        i32s %0 = alloc(8);
        i32s %0 = load_arg();
        {
            goto lb{X}
            lb{X}
            {
                goto lb{X}
                fn bar1() -> i0
                {
                    {
                        return num? 1;
                    }
                }
                lb{X}
                i0t* %1 = &(bar1() -> i0);
                return i0t* %1;
            }
            lb{X}
            {
                goto lb{X}
                fn baz2() -> i0
                {
                    {
                        return num? 2;
                    }
                }
                lb{X}
                i0t* %2 = &(baz2() -> i0);
                return i0t* %2;
            }
            goto lb{X}
            lb{X}
            i32t %4 = cnst? 1 as i32;
            i8t %3 = i32s %0 < i32t %4;
            if i8t %3, goto lb{X} else goto lb{X}
            lb{X}
            goto lb{X}
            lb{X}
            i32t %6 = cnst? 1 as i32;
            i8t %5 = i32s %0 > i32t %6;
            if i8t %5, goto lb{X} else goto lb{X}
            lb{X}
            goto lb{X}
            lb{X}
            goto lb{X}
            lb{X}
        }
    }
    start {
        {
            {
                i0t* %7 = call factory0(i32 k) -> i0*, argc args(num? 0,);
                i64t %8 = call i0t* %7, argc args();
                exit i64t %8;
            }
        }
    }
}
: