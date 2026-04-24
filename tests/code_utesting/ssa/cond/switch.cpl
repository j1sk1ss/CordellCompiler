start() {
    i32 a = 0;
    switch 1; {
        case 1; {
            a = 1;
        }
        case 2; {
            a = 2;
            break;
        }
        default {
            a = 3;
        }
    }
    i32 b = a;
}

:/ OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i32t %2 = i8n 0 as i32;
                i32s %16 = i32t %2;
                goto lb12;
                lb14:
                {
                    i32t %3 = i8n 1 as i32;
                    i32s %17 = i32t %3;
                    future: i32s %18 = previous: i32s %17;
                }
                lb21:
                [i32s %0] i32s %18 = phi(set [%17, bb2][%16, bb12]);
                {
                    i32t %4 = i8n 2 as i32;
                    i32s %19 = i32t %4;
                    // break;
                    future: i32s %20 = previous: i32s %19;
                    goto lb13;
                [unused] }
                lb29:
                {
                    i32t %5 = i8n 3 as i32;
                    i32s %22 = i32t %5;
                }
                future: i32s %20 = previous: i32s %22;
                goto lb13;
                lb12:
                i64t %6 = csu8 1 as i64;
                i64t %8 = i8n 1 as i64;
                i8t %7 = i64t %8 < i64t %6;
                if i8t %7, goto lb38, else goto lb39;
                lb38:
                goto lb29;
                lb39:
                i64t %10 = i8n 1 as i64;
                i8t %9 = i64t %10 > i64t %6;
                if i8t %9, goto lb40, else goto lb41;
                lb40:
                i64t %11 = csu8 2 as i64;
                i64t %13 = i8n 1 as i64;
                i8t %12 = i64t %13 < i64t %11;
                if i8t %12, goto lb48, else goto lb49;
                lb48:
                goto lb29;
                lb49:
                i64t %15 = i8n 1 as i64;
                i8t %14 = i64t %15 > i64t %11;
                if i8t %14, goto lb50, else goto lb51;
                lb50:
                goto lb29;
                lb51:
                future: i32s %18 = previous: i32s %16;
                goto lb21;
                lb41:
                goto lb14;
                lb13:
                [i32s %0] i32s %20 = phi(set [%19, bb3][%22, bb4]);
                i32s %1 = alloc(8);
                i32s %21 = i32s %20;
            }
        }
    }
}
/: