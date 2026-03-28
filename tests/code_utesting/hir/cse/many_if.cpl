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
                if num? 1, goto lb10, else goto lb11;
                lb10:
                {
                    i32s %0 = alloc(8);
                    i64t %4 = num? 10 + num? 10;
                    i32t %3 = i64t %4 as i32;
                    i32s %17 = i32t %3;
                    [unused] i64t %6 = num? 10 + num? 10;
                    i32t %5 = i64t %4 as i32;
                    exit i32t %5;
                }
                goto lb12;
                lb11:
                {
                    if num? 2, goto lb43, else goto lb44;
                    lb43:
                    {
                        i32s %1 = alloc(8);
                        i64t %8 = num? 10 + num? 10;
                        i32t %7 = i64t %8 as i32;
                        i32s %16 = i32t %7;
                        [unused] i64t %10 = num? 10 + num? 10;
                        i32t %9 = i64t %8 as i32;
                        exit i32t %9;
                    }
                    goto lb45;
                    lb44:
                    {
                        i32s %2 = alloc(8);
                        i64t %12 = num? 10 + num? 10;
                        i32t %11 = i64t %12 as i32;
                        i32s %15 = i32t %11;
                        [unused] i64t %14 = num? 10 + num? 10;
                        i32t %13 = i64t %12 as i32;
                        exit i32t %13;
                    }
                    goto lb45;
                    lb45:
                [unused] }
                [unused] goto lb12;
                lb12:
            [unused] }
        [unused] }
    [unused] }
}
: