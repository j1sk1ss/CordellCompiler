start() {
    i32 a = (10 + 10) as i32;
    if 1; {
        i32 b = (10 + 10) as i32;
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
                i64t %3 = num? 10 + num? 10;
                i32t %2 = i64t %3 as i32;
                i32s %6 = i32t %2;
                if num? 1, goto lb{X} else goto lb{X}
                lb{X}
                {
                    i32s %1 = alloc(8);
                    [unused] i64t %5 = num? 10 + num? 10;
                    i32t %4 = i64t %3 as i32;
                    i32s %7 = i32t %4;
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