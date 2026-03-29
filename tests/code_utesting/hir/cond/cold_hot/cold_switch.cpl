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
                i8t %0 = i64t %1 < cnst? 2;
                if i8t %0, goto lb46, else goto lb47;
                lb46:
                i64t %3 = i8n 1;
                i8t %2 = i64t %3 < cnst? 1;
                if i8t %2, goto lb53, else goto lb54;
                lb53:
                goto lb36;
                lb54:
                i64t %5 = i8n 1;
                i8t %4 = i64t %5 > cnst? 1;
                if i8t %4, goto lb55, else goto lb56;
                lb55:
                goto lb36;
                lb56:
                goto lb12;
                lb47:
                i64t %7 = i8n 1;
                i8t %6 = i64t %7 > cnst? 2;
                if i8t %6, goto lb48, else goto lb49;
                lb48:
                i64t %9 = i8n 1;
                i8t %8 = i64t %9 < cnst? 3;
                if i8t %8, goto lb66, else goto lb67;
                lb66:
                goto lb36;
                lb67:
                i64t %11 = i8n 1;
                i8t %10 = i64t %11 > cnst? 3;
                if i8t %10, goto lb68, else goto lb69;
                lb68:
                goto lb36;
                lb69:
                goto lb28;
                lb49:
                goto lb20;
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