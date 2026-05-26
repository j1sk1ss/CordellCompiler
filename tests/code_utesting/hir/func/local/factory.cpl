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
        i32s %0 = alloc;
        i32s %0 = load_arg();
        {
            goto lb11;
            lb13:
            {
                goto lb16;
                fn bar1() -> i64
                {
                    {
                        i64t %1 = i8n 1 as i64;
                        return i64t %1;
                    }
                }
                lb16:
                i0t* %2 = &(bar1() -> i64);
                return i0t* %2;
            }
            lb29:
            {
                goto lb32;
                fn baz2() -> i64
                {
                    {
                        i64t %3 = i8n 2 as i64;
                        return i64t %3;
                    }
                }
                lb32:
                i0t* %4 = &(baz2() -> i64);
                return i0t* %4;
            }
            goto lb12;
            lb11:
            i64t %5 = i32s %0 as i64;
            i64t %6 = csu8 1 as i64;
            i8t %7 = i64t %5 < i64t %6;
            if i8t %7, goto lb48, else goto lb49;
            lb48:
            goto lb29;
            lb49:
            i8t %8 = i64t %5 > i64t %6;
            if i8t %8, goto lb50, else goto lb51;
            lb50:
            goto lb29;
            lb51:
            goto lb13;
            lb12:
        }
    }
    fn _main()
    {
        {
            i32t %10 = i8n 0 as i32;
            use i32t %10;
            i0t* %11 = factory0(i32t %10);
            i64t %12 = i0t* %11();
            i8t %9 = i64t %12 as i8;
            u8t %13 = i8t %9 as u8;
            exit u8t %13;
        }
    }
}
/: