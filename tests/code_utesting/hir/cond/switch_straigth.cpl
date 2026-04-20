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
                i64t %2 = csu8 1 as i64;
                i64t %3 = i32s %0 as i64;
                i8t %1 = i64t %3 == i64t %2;
                if i8t %1, goto lb11, else goto lb33;
                lb33:
                i64t %5 = csu8 2 as i64;
                i64t %6 = i32s %0 as i64;
                i8t %4 = i64t %6 == i64t %5;
                if i8t %4, goto lb15, else goto lb38;
                lb38:
                i64t %8 = csu8 3 as i64;
                i64t %9 = i32s %0 as i64;
                i8t %7 = i64t %9 == i64t %8;
                if i8t %7, goto lb19, else goto lb43;
                lb43:
                i64t %11 = csu8 4 as i64;
                i64t %12 = i32s %0 as i64;
                i8t %10 = i64t %12 == i64t %11;
                if i8t %10, goto lb23, else goto lb48;
                lb48:
                goto lb27;
                lb10:
                exit i8n 1;
            }
        }
    }
}
/: