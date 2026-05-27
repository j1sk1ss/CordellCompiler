@[inline(model)] function small() -> i32 {
    return 100;
}

@[inline(model)] function medium(i32 a) -> i32 {
    i32 b = a * 100;
    if b > 10; return 0;
    else b -= 1;
    return b;
}

start() {
    i32 a = small();
    i32 b = medium(a);
}

:/ OUTPUT
{
    fn small0() -> i32
    {
        {
            i32t %4 = i8n 100 as i32;
            return i32t %4;
        }
    }
    fn medium1(i32 a) -> i32
    {
        i32s %0 = alloc;
        i32s %0 = load_arg();
        {
            i32s %1 = alloc;
            i32t %6 = i8n 100 as i32;
            i32t %5 = i32s %0 * i32t %6;
            i32s %1 = i32t %5;
            i32t %8 = i8n 10 as i32;
            i32t %7 = i32s %1 > i32t %8;
            if i32t %7, goto lb27, else goto lb28;
            lb27:
            {
                i32t %9 = i8n 0 as i32;
                return i32t %9;
            }
            goto lb29;
            lb28:
            {
                i32t %11 = i8n 1 as i32;
                i32t %10 = i32s %1 - i32t %11;
                i32s %1 = i32t %10;
            }
            goto lb29;
            lb29:
            return i32s %1;
        }
    }
    fn _main()
    {
        {
            i32s %2 = alloc;
            i32t %4 = i8n 100 as i32;
            i32t %12 = i32t %4;
            goto lb71;
            lb71:
            [unused] i32t %12 = small0();
            i32s %2 = i32t %12;
            i32s %3 = alloc;
            use i32s %2;
            i32s %0 = i32s %2;
            i32s %1 = alloc;
            i32t %6 = i8n 100 as i32;
            i32t %5 = i32s %0 * i32t %6;
            i32s %1 = i32t %5;
            i32t %8 = i8n 10 as i32;
            i32t %7 = i32s %1 > i32t %8;
            if i32t %7, goto lb118, else goto lb119;
            lb118:
            {
                i32t %9 = i8n 0 as i32;
                i32t %13 = i32t %9;
                goto lb82;
            }
            goto lb120;
            lb119:
            {
                i32t %11 = i8n 1 as i32;
                i32t %10 = i32s %1 - i32t %11;
                i32s %1 = i32t %10;
            }
            goto lb120;
            lb120:
            i32t %13 = i32s %1;
            goto lb82;
            lb82:
            [unused] i32t %13 = medium1(i32s %2);
            i32s %3 = i32t %13;
        }
    }
}
/: