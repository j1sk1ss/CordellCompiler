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
                return i8n 1;
            }
            goto lb9;
            lb9:
            return i8n 2;
        }
    }
    [unused]
    start {
        {
            {
                i32s %0 = alloc(8);
                if i8n 1, goto lb41, else goto lb42;
                lb41:
                {
                    i32t %1 = i8n 1;
                    goto lb31;
                }
                goto lb42;
                lb42:
                i32t %1 = i8n 2;
                goto lb31;
                lb31:
                [unused] i32t %1 = call foo0() -> i32, argc args();
                i32s %0 = i32t %1;
            }
        }
    }
}
/: