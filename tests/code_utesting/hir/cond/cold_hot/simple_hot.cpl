start(i32 a) {
    i32 b = a + 10;
    @[hot] if b == 20; {
        a += 1;
        exit b;
    }
    else {
        exit a;
    }
    i32 d = b + a;
}

: OUTPUT
{
    start {
        {
            i32s %0 = alloc(8);
            i32s %0 = load_starg();
            {
                i32s %1 = alloc(8);
                i64t %4 = i32s %0 as i64;
                i64t %3 = i64t %4 + num? 10;
                i32t %5 = i64t %3 as i32;
                i32s %1 = i32t %5;
                i64t %7 = i32s %1 as i64;
                i64t %6 = i64t %7 == num? 20;
                if i64t %6, goto lb{X} else goto lb{X}
                lb{X}
                {
                    i32t %9 = num? 1 as i32;
                    i32t %8 = i32s %0 + i32t %9;
                    i32s %0 = i32t %8;
                    exit i32s %1;
                }
                goto lb{X}
                lb{X}
                i32s %2 = alloc(8);
                i32t %10 = i32s %1 + i32s %0;
                i32s %2 = i32t %10;
            }
            exit cnst? 0;
            lb{X}
            {
                exit i32s %0;
            }
            goto lb{X}
        }
    }
}
: