start() {
    @[no_fall]
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
            i64t %4 = csu8 2 as i64;
            i64t %6 = i8n 1 as i64;
            i8t %5 = i64t %6 < i64t %4;
            if i8t %5, goto lb36, else goto lb37;
            lb36:
            i64t %7 = csu8 1 as i64;
            i64t %9 = i8n 1 as i64;
            i8t %8 = i64t %9 < i64t %7;
            if i8t %8, goto lb44, else goto lb45;
            lb44:
            goto lb28;
            lb45:
            i64t %11 = i8n 1 as i64;
            i8t %10 = i64t %11 > i64t %7;
            if i8t %10, goto lb46, else goto lb47;
            lb46:
            goto lb28;
            lb47:
            goto lb10;
            lb37:
            i64t %13 = i8n 1 as i64;
            i8t %12 = i64t %13 > i64t %4;
            if i8t %12, goto lb38, else goto lb39;
            lb38:
            i64t %14 = csu8 3 as i64;
            i64t %16 = i8n 1 as i64;
            i8t %15 = i64t %16 < i64t %14;
            if i8t %15, goto lb56, else goto lb57;
            lb56:
            goto lb28;
            lb57:
            i64t %18 = i8n 1 as i64;
            i8t %17 = i64t %18 > i64t %14;
            if i8t %17, goto lb58, else goto lb59;
            lb58:
            goto lb28;
            lb59:
            goto lb22;
            lb39:
            goto lb16;
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