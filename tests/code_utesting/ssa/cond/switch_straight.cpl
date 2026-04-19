start() {
    @[straight] switch 1; {
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
                lb16:
                {
                }
                lb20:
                {
                }
                goto lb11;
                lb10:
                i64t %1 = i8n 1;
                i8t %0 = i64t %1 == cnst? 1;
                if i8t %0, goto lb12, else goto lb26;
                lb26:
                i64t %3 = i8n 1;
                i8t %2 = i64t %3 == cnst? 2;
                if i8t %2, goto lb16, else goto lb30;
                lb30:
                goto lb20;
                lb11:
            }
        }
    }
}
/: