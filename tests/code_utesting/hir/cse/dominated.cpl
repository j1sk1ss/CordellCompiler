start() {
    i32 a = (10 + 10) as i32;
    if 1; {
        i32 b = (10 + 10) as i32;
        exit b as u8;
    }
    exit a as u8;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i32s %0 = alloc;
            i8t %3 = i8n 10 + i8n 10;
            i32t %2 = i8t %3 as i32;
            i32s %8 = i32t %2;
            if i8n 1, goto lb14, else goto lb16;
            lb14:
            {
                i32s %1 = alloc;
                [unused] i8t %5 = i8n 10 + i8n 10;
                i32t %4 = i8t %3 as i32;
                i32s %9 = i32t %4;
                u8t %6 = i32s %9 as u8;
                exit u8t %6;
            }
            goto lb16;
            lb16:
            u8t %7 = i32s %8 as u8;
            exit u8t %7;
        }
    }
}
/: