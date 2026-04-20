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
                goto lb7;
                lb9:
                {
                }
                lb13:
                {
                }
                goto lb8;
                lb7:
                i64t %1 = i8n 1;
                i8t %0 = i64t %1 < cnst? 1;
                if i8t %0, goto lb19, else goto lb20;
                lb19:
                goto lb13;
                lb20:
                i64t %3 = i8n 1;
                i8t %2 = i64t %3 > cnst? 1;
                if i8t %2, goto lb21, else goto lb22;
                lb21:
                goto lb13;
                lb22:
                goto lb9;
                lb8:
                goto lb30;
                lb32:
                {
                }
                lb36:
                {
                }
                goto lb31;
                lb30:
                i64t %5 = i8n 1;
                i8t %4 = i64t %5 == cnst? 1;
                if i8t %4, goto lb32, else goto lb42;
                lb42:
                goto lb36;
                lb31:
            }
        }
    }
}
/: