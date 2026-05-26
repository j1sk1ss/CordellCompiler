start() {
    i32 tag;
    @[no_fall]
    @[straight]
    switch tag; {
        case 1; { exit 1; }
        default { exit 0; }
    }
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i32s %0 = alloc;
            goto lb9;
            lb11:
            {
                u8t %1 = i8n 1 as u8;
                exit u8t %1;
            }
            goto lb10;
            lb17:
            {
                u8t %2 = i8n 0 as u8;
                exit u8t %2;
            }
            goto lb10;
            goto lb10;
            lb9:
            i64t %4 = csu8 1 as i64;
            i64t %5 = i32s %0 as i64;
            i8t %3 = i64t %5 == i64t %4;
            if i8t %3, goto lb11, else goto lb25;
            lb25:
            goto lb17;
            lb10:
        }
    }
}
/: