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
    start {
        {
            {
                goto lb8;
                lb20:
                {
                    exit i8n 3;
                }
                goto lb9;
                lb25:
                {
                    exit i8n 4;
                }
                goto lb9;
                goto lb9;
                lb8:
                i64t %1 = csu8 1 as i64;
                i64t %2 = i8n 1 as i64;
                i8t %0 = i64t %2 == i64t %1;
                if i8t %0, goto lb10, else goto lb32;
                lb32:
                i64t %4 = csu8 2 as i64;
                i64t %5 = i8n 1 as i64;
                i8t %3 = i64t %5 == i64t %4;
                if i8t %3, goto lb15, else goto lb37;
                lb37:
                i64t %7 = csu8 3 as i64;
                i64t %8 = i8n 1 as i64;
                i8t %6 = i64t %8 == i64t %7;
                if i8t %6, goto lb20, else goto lb42;
                lb42:
                goto lb25;
                lb9:
            }
            exit csu8 0;
            lb10:
            {
                exit i8n 1;
            }
            goto lb9;
            lb15:
            {
                exit i8n 2;
            }
            goto lb9;
        }
    }
}
/: