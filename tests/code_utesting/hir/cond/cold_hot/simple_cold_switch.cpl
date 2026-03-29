start() {
    @[no_fall]
    switch 1; {
        @[cold] case 1; { exit 1; }
        default { exit 2; }
    }
}

:/ OUTPUT
{
    start {
        {
            {
                goto lb10;
                lb20:
                {
                    exit i8n 2;
                }
                goto lb11;
                goto lb11;
                lb10:
                i64t %1 = i8n 1;
                i8t %0 = i64t %1 < cnst? 1;
                if i8t %0, goto lb30, else goto lb31;
                lb30:
                goto lb20;
                lb31:
                i64t %3 = i8n 1;
                i8t %2 = i64t %3 > cnst? 1;
                if i8t %2, goto lb32, else goto lb33;
                lb32:
                goto lb20;
                lb33:
                goto lb12;
                lb11:
            }
            exit cnst? 0;
            lb12:
            {
                exit i8n 1;
            }
            goto lb11;
        }
    }
}
/: