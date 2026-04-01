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
                i32s %14 = i32t %2;
                goto lb18;
                lb20:
                {
                    i32t %3 = i8n 1 as i32;
                    i32s %15 = i32t %3;
                    future: i32s %16 = previous: i32s %15;
                }
                lb31:
                [i32s %0] i32s %16 = phi(set [%14, bb13][%15, bb2]);
                {
                    i32t %4 = i8n 2 as i32;
                    i32s %17 = i32t %4;
                    // break;
                    future: i32s %18 = previous: i32s %17;
                    goto lb19;
                }
                lb43:
                {
                    i32t %5 = i8n 3 as i32;
                    i32s %20 = i32t %5;
                }
                future: i32s %18 = previous: i32s %20;
                goto lb19;
                lb18:
                i64t %7 = i8n 1;
                i8t %6 = i64t %7 < cnst? 1;
                if i8t %6, goto lb56, else goto lb57;
                lb56:
                goto lb43;
                lb57:
                i64t %9 = i8n 1;
                i8t %8 = i64t %9 > cnst? 1;
                if i8t %8, goto lb58, else goto lb59;
                lb58:
                i64t %11 = i8n 1;
                i8t %10 = i64t %11 < cnst? 2;
                if i8t %10, goto lb66, else goto lb67;
                lb66:
                goto lb43;
                lb67:
                i64t %13 = i8n 1;
                i8t %12 = i64t %13 > cnst? 2;
                if i8t %12, goto lb68, else goto lb69;
                lb68:
                goto lb43;
                lb69:
                future: i32s %16 = previous: i32s %14;
                goto lb31;
                lb59:
                goto lb20;
                lb19:
                [i32s %0] i32s %18 = phi(set [%20, bb5][%17, bb3]);
                i32s %1 = alloc(8);
                i32s %19 = i32s %18;
            }
        }
    }
}
/: