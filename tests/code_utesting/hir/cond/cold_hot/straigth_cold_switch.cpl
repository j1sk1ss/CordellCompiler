start() {
    @[no_fall]
    @[straight]
    switch 1; {
        @[cold] case 1; { exit 1; }
        @[cold] case 2; { exit 2; }
        case 3; { exit 3; }
        default { exit 4; }
    }
}

:/ OUTPUT
{
    fn _main()
    {
        {
            goto lb8;
            lb22:
            {
                u8t %2 = i8n 3 as u8;
                exit u8t %2;
            }
            goto lb9;
            lb28:
            {
                u8t %3 = i8n 4 as u8;
                exit u8t %3;
            }
            goto lb9;
            goto lb9;
            lb8:
            i64t %5 = csu8 1 as i64;
            i64t %6 = i8n 1 as i64;
            i8t %4 = i64t %6 == i64t %5;
            if i8t %4, goto lb10, else goto lb36;
            lb36:
            i64t %8 = csu8 2 as i64;
            i64t %9 = i8n 1 as i64;
            i8t %7 = i64t %9 == i64t %8;
            if i8t %7, goto lb16, else goto lb41;
            lb41:
            i64t %11 = csu8 3 as i64;
            i64t %12 = i8n 1 as i64;
            i8t %10 = i64t %12 == i64t %11;
            if i8t %10, goto lb22, else goto lb46;
            lb46:
            goto lb28;
            lb9:
        }
        exit csu8 0;
        lb10:
        {
            u8t %0 = i8n 1 as u8;
            exit u8t %0;
        }
        goto lb9;
        lb16:
        {
            u8t %1 = i8n 2 as u8;
            exit u8t %1;
        }
        goto lb9;
    }
}
/: