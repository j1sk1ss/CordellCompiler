: BUG :
start() {
    i32 d = 0;
    while 1; {
        i32 c = 10 + 10;
        d += c;
        if d; {
            break;
        }
    }

    exit 1;
}

: OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i32t %2 = num? 0 as i32;
                i32s %6 = i32t %2;
                future{X} i32s %8 = previous{X} i32s %1;
                future{X} i32s %7 = previous{X} i32s %6;
                i64t %3 = num? 10 + num? 10;
                i32t %4 = i64t %3 as i32;
                lb{X}
                [i32s %0] i32s %7 = phi(set [{X}][{X}]);
                [i32s %1] i32s %8 = phi(set [{X}][{X}]);
                future{X} i32s %9 = previous{X} i32s {X};
                future{X} i32s %10 = previous{X} i32s {X};
                if num? 1, goto lb{X} else goto lb{X}
                lb{X}
                {
                    i32s %8 = alloc(8);
                    i32s %11 = i32t %4;
                    i32t %5 = i32s %7 + i32s %11;
                    i32s %12 = i32t %5;
                    if i32s %12, goto lb{X} else goto lb{X}
                    lb{X}
                    {
                        // break;
                        future{X} i32s %9 = previous{X} i32s {X};
                        future{X} i32s %10 = previous{X} i32s {X};
                        goto lb{X}
                    }
                    goto lb{X}
                    lb{X}
                }
                future{X} i32s %7 = previous{X} i32s {X};
                future{X} i32s %8 = previous{X} i32s {X};
                goto lb{X}
                lb{X}
                [i32s %0] i32s %9 = phi(set [{X}][{X}]);
                [i32s %1] i32s %10 = phi(set [{X}][{X}]);
                exit num? 1;
            }
        }
    }
}
: