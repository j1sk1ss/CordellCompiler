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
                i64t %1 = i8n 1;
                i8t %0 = i64t %1 == cnst? 1;
                if i8t %0, goto lb10, else goto lb32;
                lb32:
                i64t %3 = i8n 1;
                i8t %2 = i64t %3 == cnst? 2;
                if i8t %2, goto lb15, else goto lb36;
                lb36:
                i64t %5 = i8n 1;
                i8t %4 = i64t %5 == cnst? 3;
                if i8t %4, goto lb20, else goto lb40;
                lb40:
                goto lb25;
                lb9:
            }
            exit cnst? 0;
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