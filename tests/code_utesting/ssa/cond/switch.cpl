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
                i32t %2 = num? 0 as i32;
                i32s %10 = i32t %2;
                goto lb{X}
                lb{X}
                {
                    i32t %3 = num? 1 as i32;
                    i32s %11 = i32t %3;
                    future: i32s %12 = previous: i32s %11;
                }
                lb{X}
                [i32s %0] i32s %12 = phi(set [{X}][{X}]);
                {
                    i32t %4 = num? 2 as i32;
                    i32s %13 = i32t %4;
                    // break;
                    future: i32s %14 = previous: i32s %13;
                    goto lb{X}
                }
                lb{X}
                {
                    i32t %5 = num? 3 as i32;
                    i32s %16 = i32t %5;
                }
                future: i32s %14 = previous: i32s %16;
                goto lb{X}
                lb{X}
                i8t %6 = num? 1 < cnst? 1;
                if i8t %6, goto lb{X} else goto lb{X}
                lb{X}
                goto lb{X}
                lb{X}
                i8t %7 = num? 1 > cnst? 1;
                if i8t %7, goto lb{X} else goto lb{X}
                lb{X}
                i8t %8 = num? 1 < cnst? 2;
                if i8t %8, goto lb{X} else goto lb{X}
                lb{X}
                goto lb{X}
                lb{X}
                i8t %9 = num? 1 > cnst? 2;
                if i8t %9, goto lb{X} else goto lb{X}
                lb{X}
                goto lb{X}
                lb{X}
                future: i32s %12 = previous: i32s %10;
                goto lb{X}
                lb{X}
                goto lb{X}
                lb{X}
                [i32s %0] i32s %14 = phi(set [{X}][{X}]);
                i32s %1 = alloc(8);
                i32s %15 = i32s %14;
            }
        }
    }
}
/:
