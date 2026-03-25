start() {
    i32 a = 10 + 10;
    if 1; {
        i32 b = 10 + 10;
        exit b;
    }
    exit a;
}

: OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i64t %2 = num? 10 + num? 10;
                i32t %3 = i64t %2 as i32;
                i32s %6 = i32t %3;
                if num? 1, goto lb{X} else goto lb{X}
                lb{X}
                {
                    i32s %1 = alloc(8);
                    [unused] i64t %4 = num? 10 + num? 10;
                    i32t %5 = i64t %2 as i32;
                    i32s %7 = i32t %5;
                    exit i32s %7;
                }
                goto lb{X}
                lb{X}
                exit i32s %6;
            }
        }
    }
}
: