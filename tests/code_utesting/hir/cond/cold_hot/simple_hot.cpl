start(i32 a) {
    i32 b = a + 10 as i32;
    @[hot] if b == 20 as i32; {
        a += 1 as i32;
        exit b;
    }
    else {
        exit a;
    }
    i32 d = b + a;
}

:/ OUTPUT
{
    start {
        {
            i32s %0 = alloc(8);
            i32s %0 = load_starg();
            {
                i32s %1 = alloc(8);
                i32t %3 = i8n 10 as i32;
                i32t %4 = i32s %0 + i32t %3;
                i32s %1 = i32t %4;
                i32t %5 = i8n 20 as i32;
                i32t %6 = i32s %1 == i32t %5;
                if i32t %6, goto lb26, else goto lb27;
                lb26:
                {
                    i32t %7 = i8n 1 as i32;
                    i32t %8 = i32s %0 + i32t %7;
                    i32s %0 = i32t %8;
                    exit i32s %1;
                }
                goto lb28;
                lb28:
                i32s %2 = alloc(8);
                i32t %9 = i32s %1 + i32s %0;
                i32s %2 = i32t %9;
            }
            exit cnst? 0;
            lb27:
            {
                exit i32s %0;
            }
            goto lb28;
        }
    }
}
/: