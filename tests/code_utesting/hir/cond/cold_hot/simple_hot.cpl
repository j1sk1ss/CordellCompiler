start(i32 a) {
    i32 b = a + 10 as i32;
    @[hot] if b == 20 as i32; {
        a += 1 as i32;
        exit b as u8;
    }
    else {
        exit a as u8;
    }
    i32 d = b + a;
}

:/ OUTPUT
{
    fn _main(i32 a)
    {
        i32s %0 = alloc;
        i32s %0 = load_starg();
        {
            i32s %1 = alloc;
            i32t %3 = i8n 10 as i32;
            i32t %4 = i32s %0 + i32t %3;
            i32s %1 = i32t %4;
            i32t %5 = i8n 20 as i32;
            i32t %6 = i32s %1 == i32t %5;
            if i32t %6, goto lb17, else goto lb18;
            lb17:
            {
                i32t %7 = i8n 1 as i32;
                i32t %8 = i32s %0 + i32t %7;
                i32s %0 = i32t %8;
                u8t %9 = i32s %1 as u8;
                exit u8t %9;
            }
            goto lb19;
            lb19:
            i32s %2 = alloc;
            i32t %11 = i32s %1 + i32s %0;
            i32s %2 = i32t %11;
        }
        exit csu8 0;
        lb18:
        {
            u8t %10 = i32s %0 as u8;
            exit u8t %10;
        }
        goto lb19;
    }
}
/: