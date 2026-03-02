{
    start() {
        if 1; {
            i32 a = 10 + 10;
            exit 10 + 10;
        }
        else if 2; {
            i32 a = 10 + 10;
            exit 10 + 10;
        }
        else {
            i32 a = 10 + 10;
            exit 10 + 10;
        }
    }
}

: OUTPUT
{
    start {
        {
            {
                if num? 1, goto lb4, else goto lb5;
                lb4:
                {
                    i32s %0 = alloc(8);
                    i64t %3 = num? 10 + num? 10;
                    i32t %4 = i64t %3 as i32;
                    i32s %14 = i32t %4;
                    [unused] i64t %5 = num? 10 + num? 10;
                    exit i64t %3;
                }
                goto lb6;
                lb5:
                {
                    if num? 2, goto lb20, else goto lb21;
                    lb20:
                    {
                        i32s %1 = alloc(8);
                        i64t %6 = num? 10 + num? 10;
                        i32t %7 = i64t %6 as i32;
                        i32s %13 = i32t %7;
                        [unused] i64t %8 = num? 10 + num? 10;
                        exit i64t %6;
                    }
                    goto lb22;
                    lb21:
                    {
                        i32s %2 = alloc(8);
                        i64t %9 = num? 10 + num? 10;
                        i32t %10 = i64t %9 as i32;
                        i32s %12 = i32t %10;
                        [unused] i64t %11 = num? 10 + num? 10;
                        exit i64t %9;
                    }
                    goto lb22;
                    lb22:
                }
                goto lb6;
                lb6:
            }
        }
    }
}
: