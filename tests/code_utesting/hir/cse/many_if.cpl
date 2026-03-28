start() {
    if 1; {
        i32 a = (10 + 10) as i32;
        exit (10 + 10) as i32;
    }
    else if 2; {
        i32 a = (10 + 10) as i32;
        exit (10 + 10) as i32;
    }
    else {
        i32 a = (10 + 10) as i32;
        exit (10 + 10) as i32;
    }
}

: OUTPUT
{
    start {
        {
            {
                if num? 1, goto lb{X} else goto lb{X}
                lb{X}
                {
                    i32s %0 = alloc(8);
                    i64t %4 = num? 10 + num? 10;
                    i32t %3 = i64t %4 as i32;
                    i32s %17 = i32t %3;
                    [unused] i64t %5 = num? 10 + num? 10;
                    exit i64t %3;
                }
                goto lb{X}
                lb{X}
                {
                    if num? 2, goto lb{X} else goto lb{X}
                    lb{X}
                    {
                        i32s %1 = alloc(8);
                        i64t %6 = num? 10 + num? 10;
                        i32t %7 = i64t %6 as i32;
                        i32s %13 = i32t %7;
                        [unused] i64t %8 = num? 10 + num? 10;
                        exit i64t %6;
                    }
                    goto lb{X}
                    lb{X}
                    {
                        i32s %2 = alloc(8);
                        i64t %9 = num? 10 + num? 10;
                        i32t %10 = i64t %9 as i32;
                        i32s %12 = i32t %10;
                        [unused] i64t %11 = num? 10 + num? 10;
                        exit i64t %9;
                    }
                    goto lb{X}
                    lb{X}
                [unused] }
                [unused] goto lb{X}
                lb{X}
            [unused] }
        [unused] }
    [unused] }
}
: