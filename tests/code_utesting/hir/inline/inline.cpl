function foo(i32 a) -> i32 {
    arr array[10, i32] = { 0 };
    if a == 1; {
        return 0;
    }
    else {
        return 1;
    }
}

start() {
    i32 a = foo(10);
}

:/ OUTPUT
{
    fn foo0(i32 a) -> i32
    {
        i32s %0 = alloc;
        i32s %0 = load_arg();
        {
            arrs %1 = arr_alloc(i8n 10);
            i32t %4 = i8n 1 as i32;
            i32t %3 = i32s %0 == i32t %4;
            if i32t %3, goto lb16, else goto lb17;
            lb16:
            {
                i32t %5 = i8n 0 as i32;
                return i32t %5;
            }
            goto lb18;
            lb17:
            {
                i32t %6 = i8n 1 as i32;
                return i32t %6;
            }
            goto lb18;
            lb18:
        }
    }
    fn _main()
    {
        {
            i32s %2 = alloc;
            i32s %0 = i8n 10;
            arrs %1 = arr_alloc(i8n 10);
            i32t %4 = i8n 1 as i32;
            i32t %3 = i32s %0 == i32t %4;
            if i32t %3, goto lb80, else goto lb81;
            lb80:
            {
                i32t %5 = i8n 0 as i32;
                i32t %7 = i32t %5;
                goto lb53;
            }
            goto lb82;
            lb81:
            {
                i32t %6 = i8n 1 as i32;
                i32t %7 = i32t %6;
                goto lb53;
            }
            goto lb82;
            lb82:
            lb53:
            [unused] i32t %7 = foo0(i8n 10);
            i32s %2 = i32t %7;
        }
    }
}
/: