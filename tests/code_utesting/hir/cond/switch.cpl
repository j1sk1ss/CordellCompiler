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
                goto lb7;
                lb9:
                {
                }
                lb13:
                {
                    // break;
                    goto lb8;
                }
                lb17:
                {
                }
                goto lb8;
                lb7:
                i64t %1 = i8n 1;
                i8t %0 = i64t %1 < cnst? 1;
                if i8t %0, goto lb23, else goto lb24;
                lb23:
                goto lb17;
                lb24:
                i64t %3 = i8n 1;
                i8t %2 = i64t %3 > cnst? 1;
                if i8t %2, goto lb25, else goto lb26;
                lb25:
                i64t %5 = i8n 1;
                i8t %4 = i64t %5 < cnst? 2;
                if i8t %4, goto lb33, else goto lb34;
                lb33:
                goto lb17;
                lb34:
                i64t %7 = i8n 1;
                i8t %6 = i64t %7 > cnst? 2;
                if i8t %6, goto lb35, else goto lb36;
                lb35:
                goto lb17;
                lb36:
                goto lb13;
                lb26:
                goto lb9;
                lb8:
            }
        }
    }
}
/: