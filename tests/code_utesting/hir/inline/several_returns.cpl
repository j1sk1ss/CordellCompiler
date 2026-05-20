function foo() -> i32 {
    if 1; return 1;
    return 2;
}

start() {
    i32 a = foo();
}

:/ OUTPUT
{
    fn foo0() -> i32
    {
        {
            if i8n 1, goto lb7, else goto lb9;
            lb7:
            {
                i32t %1 = i8n 1 as i32;
                return i32t %1;
            }
            goto lb9;
            lb9:
            i32t %2 = i8n 2 as i32;
            return i32t %2;
        }
    }
    fn _main()
    {
        {
            i32s %0 = alloc;
            if i8n 1, goto lb47, else goto lb48;
            lb47:
            {
                i32t %1 = i8n 1 as i32;
                i32t %3 = i32t %1;
                goto lb33;
            }
            goto lb48;
            lb48:
            i32t %2 = i8n 2 as i32;
            i32t %3 = i32t %2;
            goto lb33;
            lb33:
            [unused] i32t %3 = foo0();
            i32s %0 = i32t %3;
        }
    }
}
/: