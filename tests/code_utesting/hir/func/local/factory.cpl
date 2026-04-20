function factory(i32 k) -> ptr i0 {
    switch k as i64; {
        case 1; {
            function bar() -> i64 {
                return 1;
            }
            return bar;
        }
        default {
            function baz() -> i64 {
                return 2;
            }
            return baz;
        }
    }
}

start() {
    exit factory(0 as i32)() as i8;
}

:/ OUTPUT
{
    fn factory0(i32 k) -> i0*
    {
        i32s %0 = alloc(8);
        i32s %0 = load_arg();
        {
            goto lb10;
            lb12:
            {
                goto lb15;
                fn bar1() -> i64
                {
                    {
                        return i8n 1;
                    }
                }
                lb15:
                i0t* %1 = &(bar1() -> i64);
                return i0t* %1;
            }
            lb27:
            {
                goto lb30;
                fn baz2() -> i64
                {
                    {
                        return i8n 2;
                    }
                }
                lb30:
                i0t* %2 = &(baz2() -> i64);
                return i0t* %2;
            }
            goto lb11;
            lb10:
            i64t %3 = i32s %0 as i64;
            i8t %4 = i64t %3 < cnst? 1;
            if i8t %4, goto lb45, else goto lb46;
            lb45:
            goto lb27;
            lb46:
            i8t %5 = i64t %3 > cnst? 1;
            if i8t %5, goto lb47, else goto lb48;
            lb47:
            goto lb27;
            lb48:
            goto lb12;
            lb11:
        }
    }
    start {
        {
            {
                i32t %7 = i8n 0 as i32;
                use i32t %7;
                i0t* %8 = call factory0(i32 k) -> i0*, argc args(i32t %7,);
                i64t %9 = call i0t* %8, argc args();
                i8t %6 = i64t %9 as i8;
                exit i8t %6;
            }
        }
    }
}
/: