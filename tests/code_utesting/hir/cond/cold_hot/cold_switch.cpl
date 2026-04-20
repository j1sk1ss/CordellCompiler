start() {
    @[no_fall]
    switch 1; {
        @[cold] case 1; { exit 1; }
        @[cold] case 2; { exit 2; }
        case 3; { exit 3; }
        default { exit 4; }
    }
}

:/ OUTPUT
{
    start {
        {
            {
                goto lb8;
                lb20:
                {
                    exit i8n 3;
                }
                goto lb9;
                lb25:
                {
                    exit i8n 4;
                }
                goto lb9;
                goto lb9;
                lb8:
                i64t %0 = csu8 2 as i64;
                i64t %2 = i8n 1 as i64;
                i8t %1 = i64t %2 < i64t %0;
                if i8t %1, goto lb32, else goto lb33;
                lb32:
                i64t %3 = csu8 1 as i64;
                i64t %5 = i8n 1 as i64;
                i8t %4 = i64t %5 < i64t %3;
                if i8t %4, goto lb40, else goto lb41;
                lb40:
                goto lb25;
                lb41:
                i64t %7 = i8n 1 as i64;
                i8t %6 = i64t %7 > i64t %3;
                if i8t %6, goto lb42, else goto lb43;
                lb42:
                goto lb25;
                lb43:
                goto lb10;
                lb33:
                i64t %9 = i8n 1 as i64;
                i8t %8 = i64t %9 > i64t %0;
                if i8t %8, goto lb34, else goto lb35;
                lb34:
                i64t %10 = csu8 3 as i64;
                i64t %12 = i8n 1 as i64;
                i8t %11 = i64t %12 < i64t %10;
                if i8t %11, goto lb52, else goto lb53;
                lb52:
                goto lb25;
                lb53:
                i64t %14 = i8n 1 as i64;
                i8t %13 = i64t %14 > i64t %10;
                if i8t %13, goto lb54, else goto lb55;
                lb54:
                goto lb25;
                lb55:
                goto lb20;
                lb35:
                goto lb15;
                lb9:
            }
            exit csu8 0;
            lb10:
            {
                exit i8n 1;
            }
            goto lb9;
            lb15:
            {
                exit i8n 2;
            }
            goto lb9;
        }
    }
}
/: