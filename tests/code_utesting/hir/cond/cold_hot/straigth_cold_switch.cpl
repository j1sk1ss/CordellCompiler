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
                goto lb10;
                lb28:
                {
                    exit i8n 3;
                }
                goto lb11;
                lb36:
                {
                    exit i8n 4;
                }
                goto lb11;
                goto lb11;
                lb10:
                i64t %1 = i8n 1;
                i8t %0 = i64t %1 == cnst? 1;
                if i8t %0, goto lb12, else goto lb46;
                lb46:
                i64t %3 = i8n 1;
                i8t %2 = i64t %3 == cnst? 2;
                if i8t %2, goto lb20, else goto lb50;
                lb50:
                i64t %5 = i8n 1;
                i8t %4 = i64t %5 == cnst? 3;
                if i8t %4, goto lb28, else goto lb54;
                lb54:
                goto lb36;
                lb11:
            }
            exit cnst? 0;
            lb12:
            {
                exit i8n 1;
            }
            goto lb11;
            lb20:
            {
                exit i8n 2;
            }
            goto lb11;
        }
    }
}
/: