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
                goto lb7;
                lb9:
                {
                }
                lb13:
                {
                }
                lb17:
                {
                }
                goto lb8;
                lb7:
                i64t %1 = csu8 1 as i64;
                i64t %2 = i8n 1 as i64;
                i8t %0 = i64t %2 == i64t %1;
                if i8t %0, goto lb9, else goto lb23;
                lb23:
                i64t %4 = csu8 2 as i64;
                i64t %5 = i8n 1 as i64;
                i8t %3 = i64t %5 == i64t %4;
                if i8t %3, goto lb13, else goto lb28;
                lb28:
                goto lb17;
                lb8:
            }
        }
    }
}
/: