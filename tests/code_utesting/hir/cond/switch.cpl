start() {
    switch 1; {
        case 1; {
        }
        case 2; {
            break;
        }
        default {
        }
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
                lb16:
                {
                    // break;
                    goto lb11;
                }
                lb22:
                {
                }
                goto lb11;
                lb10:
                i64t %1 = i8n 1;
                i8t %0 = i64t %1 < cnst? 1;
                if i8t %0, goto lb28, else goto lb29;
                lb28:
                goto lb22;
                lb29:
                i64t %3 = i8n 1;
                i8t %2 = i64t %3 > cnst? 1;
                if i8t %2, goto lb30, else goto lb31;
                lb30:
                i64t %5 = i8n 1;
                i8t %4 = i64t %5 < cnst? 2;
                if i8t %4, goto lb38, else goto lb39;
                lb38:
                goto lb22;
                lb39:
                i64t %7 = i8n 1;
                i8t %6 = i64t %7 > cnst? 2;
                if i8t %6, goto lb40, else goto lb41;
                lb40:
                goto lb22;
                lb41:
                goto lb16;
                lb31:
                goto lb12;
                lb11:
            }
        }
    }
}
/: