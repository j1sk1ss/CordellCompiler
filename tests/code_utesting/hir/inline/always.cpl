@[inline(always)]
function foo() -> i32 {
    i32 a = 100;
    i32 b = 200;
    if a > b; return 0;
    return 1;
}

@[inline(never)]
function bar() -> i32 {
    return 0;
}

start() {
    i32 a = foo();
    i32 b = bar();
}

:/ OUTPUT
{
    fn foo0() -> i32
    {
        {
            i32s %0 = alloc;
            i32t %4 = i8n 100 as i32;
            i32s %0 = i32t %4;
            i32s %1 = alloc;
            i32t %5 = u8n 200 as i32;
            i32s %1 = i32t %5;
            i32t %6 = i32s %0 > i32s %1;
            if i32t %6, goto lb17, else goto lb19;
            lb17:
            {
                i32t %7 = i8n 0 as i32;
                return i32t %7;
            }
            goto lb19;
            lb19:
            i32t %8 = i8n 1 as i32;
            return i32t %8;
        }
    }
    fn bar1() -> i32
    {
        {
            i32t %9 = i8n 0 as i32;
            return i32t %9;
        }
    }
    fn _main()
    {
        {
            i32s %2 = alloc;
            i32s %0 = alloc;
            i32t %4 = i8n 100 as i32;
            i32s %0 = i32t %4;
            i32s %1 = alloc;
            i32t %5 = u8n 200 as i32;
            i32s %1 = i32t %5;
            i32t %6 = i32s %0 > i32s %1;
            if i32t %6, goto lb89, else goto lb90;
            lb89:
            {
                i32t %7 = i8n 0 as i32;
                i32t %10 = i32t %7;
                goto lb60;
            }
            goto lb90;
            lb90:
            i32t %8 = i8n 1 as i32;
            i32t %10 = i32t %8;
            goto lb60;
            lb60:
            [unused] i32t %10 = foo0();
            i32s %2 = i32t %10;
            i32s %3 = alloc;
            i32t %11 = bar1();
            i32s %3 = i32t %11;
        }
    }
}
/: