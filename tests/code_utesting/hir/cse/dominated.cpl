start() {
    i32 a = (10 + 10) as i32;
    if 1; {
        i32 b = (10 + 10) as i32;
        exit b;
    }
    exit a;
}

:/ OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i8t %3 = i8n 10 + i8n 10;
                i32t %2 = i8t %3 as i32;
                i32s %6 = i32t %2;
                if i8n 1, goto lb23, else goto lb25;
                lb23:
                {
                    i32s %1 = alloc(8);
                    [unused] i8t %5 = i8n 10 + i8n 10;
                    i32t %4 = i8t %3 as i32;
                    i32s %7 = i32t %4;
                    exit i32s %7;
                }
                goto lb25;
                lb25:
                exit i32s %6;
            }
        }
    [unused] }
}
/: