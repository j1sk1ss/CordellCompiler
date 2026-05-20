start() {
    function local(i32 a) {
        return 1;
    }
    function local(i16 a) {
        return 2;
    }
    function local(i8 a) {
        return 3;
    }
    exit (local(1 as i32) + local(1 as i16) + local(1 as i8)) as u8;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            goto lb7;
            fn local1(i32 a)
            {
                i32s %0 = alloc;
                i32s %0 = load_arg();
                {
                    return i8n 1;
                }
            }
            lb7:
            goto lb20;
            fn local2(i16 a)
            {
                i16s %1 = alloc;
                i16s %1 = load_arg();
                {
                    return i8n 2;
                }
            }
            lb20:
            goto lb33;
            fn local3(i8 a)
            {
                i8s %2 = alloc;
                i8s %2 = load_arg();
                {
                    return i8n 3;
                }
            }
            lb33:
            i32t %4 = i8n 1 as i32;
            use i32t %4;
            i64t %5 = local1(i32t %4);
            i16t %6 = i8n 1 as i16;
            use i16t %6;
            i64t %7 = local2(i16t %6);
            i64t %8 = i64t %5 + i64t %7;
            i8t %9 = i8n 1 as i8;
            use i8t %9;
            i64t %10 = local3(i8t %9);
            i64t %11 = i64t %8 + i64t %10;
            u8t %3 = i64t %11 as u8;
            exit u8t %3;
        }
    }
}
/: