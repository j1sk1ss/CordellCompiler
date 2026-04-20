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
                i8t %0 = i64t %1 < cnst? 2;
                if i8t %0, goto lb32, else goto lb33;
                lb32:
                i64t %3 = i8n 1;
                i8t %2 = i64t %3 < cnst? 1;
                if i8t %2, goto lb39, else goto lb40;
                lb39:
                goto lb25;
                lb40:
                i64t %5 = i8n 1;
                i8t %4 = i64t %5 > cnst? 1;
                if i8t %4, goto lb41, else goto lb42;
                lb41:
                goto lb25;
                lb42:
                goto lb10;
                lb33:
                i64t %7 = i8n 1;
                i8t %6 = i64t %7 > cnst? 2;
                if i8t %6, goto lb34, else goto lb35;
                lb34:
                i64t %9 = i8n 1;
                i8t %8 = i64t %9 < cnst? 3;
                if i8t %8, goto lb52, else goto lb53;
                lb52:
                goto lb25;
                lb53:
                i64t %11 = i8n 1;
                i8t %10 = i64t %11 > cnst? 3;
                if i8t %10, goto lb54, else goto lb55;
                lb54:
                goto lb25;
                lb55:
                goto lb20;
                lb35:
                goto lb15;
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