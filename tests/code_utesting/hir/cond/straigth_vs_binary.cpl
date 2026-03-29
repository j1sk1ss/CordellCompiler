start() {
    switch 1; {
        case 1; {}
        default {}
    }

    @[straight] switch 1; {
        case 1; {}
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
                lb16:
                {
                }
                goto lb11;
                lb10:
                i64t %1 = i8n 1;
                i8t %0 = i64t %1 < cnst? 1;
                if i8t %0, goto lb22, else goto lb23;
                lb22:
                goto lb16;
                lb23:
                i64t %3 = i8n 1;
                i8t %2 = i64t %3 > cnst? 1;
                if i8t %2, goto lb24, else goto lb25;
                lb24:
                goto lb16;
                lb25:
                goto lb12;
                lb11:
                goto lb33;
                lb35:
                {
                }
                lb39:
                {
                }
                goto lb34;
                lb33:
                i64t %5 = i8n 1;
                i8t %4 = i64t %5 == cnst? 1;
                if i8t %4, goto lb35, else goto lb45;
                lb45:
                goto lb39;
                lb34:
            }
        }
    }
}
/: