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
                goto lb9;
                lb11:
                {
                }
                lb15:
                {
                }
                lb19:
                {
                }
                lb23:
                {
                }
                lb27:
                {
                }
                goto lb10;
                lb9:
                i64t %2 = i32s %0;
                i8t %1 = i64t %2 == cnst? 1;
                if i8t %1, goto lb11, else goto lb33;
                lb33:
                i64t %4 = i32s %0;
                i8t %3 = i64t %4 == cnst? 2;
                if i8t %3, goto lb15, else goto lb37;
                lb37:
                i64t %6 = i32s %0;
                i8t %5 = i64t %6 == cnst? 3;
                if i8t %5, goto lb19, else goto lb41;
                lb41:
                i64t %8 = i32s %0;
                i8t %7 = i64t %8 == cnst? 4;
                if i8t %7, goto lb23, else goto lb45;
                lb45:
                goto lb27;
                lb10:
                exit i8n 1;
            }
        }
    }
}
/: