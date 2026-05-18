start() {
    @[no_fall]
    switch 1; {
        @[cold] case 1; { exit 1; }
        default { exit 2; }
    }
}

:/ OUTPUT
{
    fn _main()
    {
        {
            goto lb8;
            lb16:
            {
                u8t %1 = i8n 2 as u8;
                exit u8t %1;
            }
            goto lb9;
            goto lb9;
            lb8:
            i64t %2 = csu8 1 as i64;
            i64t %4 = i8n 1 as i64;
            i8t %3 = i64t %4 < i64t %2;
            if i8t %3, goto lb24, else goto lb25;
            lb24:
            goto lb16;
            lb25:
            i64t %6 = i8n 1 as i64;
            i8t %5 = i64t %6 > i64t %2;
            if i8t %5, goto lb26, else goto lb27;
            lb26:
            goto lb16;
            lb27:
            goto lb10;
            lb9:
        }
        exit csu8 0;
        lb10:
        {
            u8t %0 = i8n 1 as u8;
            exit u8t %0;
        }
        goto lb9;
    }
}
/: