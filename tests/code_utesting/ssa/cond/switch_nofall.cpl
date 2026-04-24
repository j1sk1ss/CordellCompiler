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
                goto lb7;
                lb9:
                {
                }
                goto lb8;
                lb13:
                {
                }
                goto lb8;
                lb17:
                {
                }
                goto lb8;
                [unused] goto lb8;
                lb7:
                i64t %0 = csu8 1 as i64;
                i64t %2 = i8n 1 as i64;
                i8t %1 = i64t %2 < i64t %0;
                if i8t %1, goto lb23, else goto lb24;
                lb23:
                goto lb17;
                lb24:
                i64t %4 = i8n 1 as i64;
                i8t %3 = i64t %4 > i64t %0;
                if i8t %3, goto lb25, else goto lb26;
                lb25:
                i64t %5 = csu8 2 as i64;
                i64t %7 = i8n 1 as i64;
                i8t %6 = i64t %7 < i64t %5;
                if i8t %6, goto lb33, else goto lb34;
                lb33:
                goto lb17;
                lb34:
                i64t %9 = i8n 1 as i64;
                i8t %8 = i64t %9 > i64t %5;
                if i8t %8, goto lb35, else goto lb36;
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