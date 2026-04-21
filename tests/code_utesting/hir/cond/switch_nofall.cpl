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
                i64t %2 = csu8 2 as i64;
                i8t %3 = i64t %1 < i64t %2;
                if i8t %3, goto lb34, else goto lb35;
                lb34:
                i64t %4 = csu8 1 as i64;
                i8t %5 = i64t %1 < i64t %4;
                if i8t %5, goto lb41, else goto lb42;
                lb41:
                goto lb27;
                lb42:
                i8t %6 = i64t %1 > i64t %4;
                if i8t %6, goto lb43, else goto lb44;
                lb43:
                goto lb27;
                lb44:
                goto lb11;
                lb35:
                i8t %7 = i64t %1 > i64t %2;
                if i8t %7, goto lb36, else goto lb37;
                lb36:
                i64t %8 = csu8 3 as i64;
                i8t %9 = i64t %1 < i64t %8;
                if i8t %9, goto lb50, else goto lb51;
                lb50:
                goto lb27;
                lb51:
                i8t %10 = i64t %1 > i64t %8;
                if i8t %10, goto lb52, else goto lb53;
                lb52:
                i64t %11 = csu8 4 as i64;
                i8t %12 = i64t %1 < i64t %11;
                if i8t %12, goto lb58, else goto lb59;
                lb58:
                goto lb27;
                lb59:
                i8t %13 = i64t %1 > i64t %11;
                if i8t %13, goto lb60, else goto lb61;
                lb60:
                goto lb27;
                lb61:
                goto lb23;
                lb53:
                goto lb19;
                lb37:
                goto lb15;
                lb10:
                i8t %14 = i8n 1 as i8;
                exit i8t %14;
            }
        }
    }
}
/: