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
        i32s %0 = alloc(8);
        i32s %0 = load_arg();
        {
            arrs %1 = arr_alloc(i8n 10);
            i32t %4 = i8n 1 as i32;
            i32t %3 = i32s %0 == i32t %4;
            if i32t %3, goto lb20, else goto lb21;
            lb20:
            {
                return i8n 0;
            }
            goto lb22;
            lb21:
            {
                return i8n 1;
            }
            goto lb22;
            lb22:
        [unused] }
    [unused] }
    [unused]
    start {
        {
            {
                i32s %2 = alloc(8);
                i32s %0 = i8n 10;
                arrs %1 = arr_alloc(i8n 10);
                i32t %4 = i8n 1 as i32;
                i32t %3 = i32s %0 == i32t %4;
                if i32t %3, goto lb97, else goto lb98;
                lb97:
                {
                    i32t %5 = i8n 0;
                }
                goto lb99;
                lb98:
                {
                    i32t %5 = i8n 1;
                }
                goto lb99;
                lb99:
                [unused] i32t %5 = call foo0(i32 a) -> i32, argc args(i8n 10,);
                i32s %2 = i32t %5;
            }
        }
    }
}
/: