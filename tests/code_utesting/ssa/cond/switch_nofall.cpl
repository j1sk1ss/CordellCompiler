start() {
    @[no_fall] switch 1; {
        case 1; {}
        case 2; {}
        default {}
    }
}

:/ OUTPUT
{
    start {
        {
            {
                goto lb10;
                lb12:
                {
                }
                goto lb11;
                lb16:
                {
                }
                goto lb11;
                lb20:
                {
                }
                goto lb11;
                goto lb11;
                lb10:
                i64t %1 = i8n 1;
                i8t %0 = i64t %1 < cnst? 1;
                if i8t %0, goto lb26, else goto lb27;
                lb26:
                goto lb20;
                lb27:
                i64t %3 = i8n 1;
                i8t %2 = i64t %3 > cnst? 1;
                if i8t %2, goto lb28, else goto lb29;
                lb28:
                i64t %5 = i8n 1;
                i8t %4 = i64t %5 < cnst? 2;
                if i8t %4, goto lb36, else goto lb37;
                lb36:
                goto lb20;
                lb37:
                i64t %7 = i8n 1;
                i8t %6 = i64t %7 > cnst? 2;
                if i8t %6, goto lb38, else goto lb39;
                lb38:
                goto lb20;
                lb39:
                goto lb16;
                lb29:
                goto lb12;
                lb11:
            }
        }
    }
}
/: