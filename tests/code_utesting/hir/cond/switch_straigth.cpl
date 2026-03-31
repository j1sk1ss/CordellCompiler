start() {
    i32 a;
    @[straight] switch a; {
        case 1; {}
        case 2; {}
        case 3; {}
        case 4; {}
        default {}
    }
    exit 1;
}

:/ OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                goto lb12;
                lb14:
                {
                }
                lb18:
                {
                }
                lb22:
                {
                }
                lb26:
                {
                }
                lb30:
                {
                }
                goto lb13;
                lb12:
                i64t %2 = i32s %0;
                i8t %1 = i64t %2 == cnst? 1;
                if i8t %1, goto lb14, else goto lb36;
                lb36:
                i64t %4 = i32s %0;
                i8t %3 = i64t %4 == cnst? 2;
                if i8t %3, goto lb18, else goto lb40;
                lb40:
                i64t %6 = i32s %0;
                i8t %5 = i64t %6 == cnst? 3;
                if i8t %5, goto lb22, else goto lb44;
                lb44:
                i64t %8 = i32s %0;
                i8t %7 = i64t %8 == cnst? 4;
                if i8t %7, goto lb26, else goto lb48;
                lb48:
                goto lb30;
                lb13:
                exit i8n 1;
            }
        }
    }
}
/: