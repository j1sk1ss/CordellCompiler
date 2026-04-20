start() {
    i32 a;
    @[no_fall] switch a as i64; {
        case 1; {}
        case 2; {}
        case 3; {}
        case 4; {}
        default {}
    }
    exit 1 as i8;
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
                goto lb10;
                lb15:
                {
                }
                goto lb10;
                lb19:
                {
                }
                goto lb10;
                lb23:
                {
                }
                goto lb10;
                lb27:
                {
                }
                goto lb10;
                goto lb10;
                lb9:
                i64t %1 = i32s %0 as i64;
                i8t %2 = i64t %1 < cnst? 2;
                if i8t %2, goto lb34, else goto lb35;
                lb34:
                i8t %3 = i64t %1 < cnst? 1;
                if i8t %3, goto lb40, else goto lb41;
                lb40:
                goto lb27;
                lb41:
                i8t %4 = i64t %1 > cnst? 1;
                if i8t %4, goto lb42, else goto lb43;
                lb42:
                goto lb27;
                lb43:
                goto lb11;
                lb35:
                i8t %5 = i64t %1 > cnst? 2;
                if i8t %5, goto lb36, else goto lb37;
                lb36:
                i8t %6 = i64t %1 < cnst? 3;
                if i8t %6, goto lb50, else goto lb51;
                lb50:
                goto lb27;
                lb51:
                i8t %7 = i64t %1 > cnst? 3;
                if i8t %7, goto lb52, else goto lb53;
                lb52:
                i8t %8 = i64t %1 < cnst? 4;
                if i8t %8, goto lb58, else goto lb59;
                lb58:
                goto lb27;
                lb59:
                i8t %9 = i64t %1 > cnst? 4;
                if i8t %9, goto lb60, else goto lb61;
                lb60:
                goto lb27;
                lb61:
                goto lb23;
                lb53:
                goto lb19;
                lb37:
                goto lb15;
                lb10:
                i8t %10 = i8n 1 as i8;
                exit i8t %10;
            }
        }
    }
}
/: