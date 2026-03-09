{
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
}

: OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i32t %2 = num? 0 as i32;
                i32s %10 = i32t %2;
                goto lb8;
                lb10:
                {
                    i32t %3 = num? 1 as i32;
                    i32s %11 = i32t %3;
                    future: i32s %12 = previous: i32s %11;
                }
                lb16:
                [i32s %0] i32s %12 = phi(set [{X}][{X}]);
                {
                    i32t %4 = num? 2 as i32;
                    i32s %13 = i32t %4;
                    // break;
                    future: i32s %14 = previous: i32s %13;
                    goto lb9;
                }
                lb22:
                {
                    i32t %5 = num? 3 as i32;
                    i32s %16 = i32t %5;
                }
                future: i32s %14 = previous: i32s %16;
                goto lb9;
                lb8:
                i8t %6 = num? 1 < cnst? 1;
                if i8t %6, goto lb29, else goto lb30;
                lb29:
                goto lb22;
                lb30:
                i8t %7 = num? 1 > cnst? 1;
                if i8t %7, goto lb31, else goto lb32;
                lb31:
                i8t %8 = num? 1 < cnst? 2;
                if i8t %8, goto lb37, else goto lb38;
                lb37:
                goto lb22;
                lb38:
                i8t %9 = num? 1 > cnst? 2;
                if i8t %9, goto lb39, else goto lb40;
                lb39:
                goto lb22;
                lb40:
                future: i32s %12 = previous: i32s %10;
                goto lb16;
                lb32:
                goto lb10;
                lb9:
                [i32s %0] i32s %14 = phi(set [{X}][{X}]);
                i32s %1 = alloc(8);
                i32s %15 = i32s %14;
            }
        }
    }
}
: