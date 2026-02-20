{
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
}

: OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i32t %2 = num? 0 as i32;
                i32s %6 = i32t %2;
                future: i32s %8 = previous: i32s %6;
                future: i32s %7 = previous: i32s %1;
                i64t %3 = num? 10 + num? 10;
                i32t %4 = i64t %3 as i32;
                lb8:
                [i32s %1] i32s %7 = phi(set [%11, bb6][%1, bb9]);
                [i32s %0] i32s %8 = phi(set [%12, bb6][%6, bb9]);
                future: i32s %9 = previous: i32s %7;
                future: i32s %10 = previous: i32s %8;
                if num? 1, goto lb9, else goto lb10;
                lb9:
                {
                    i32s %7 = alloc(8);
                    i32s %11 = i32t %4;
                    i32t %5 = i32s %8 + i32s %11;
                    i32s %12 = i32t %5;
                    if i32s %12, goto lb23, else goto lb25;
                    lb23:
                    {
                        // break;
                        future: i32s %9 = previous: i32s %11;
                        future: i32s %10 = previous: i32s %12;
                        goto lb10;
                    }
                    goto lb25;
                    lb25:
                }
                future: i32s %7 = previous: i32s %11;
                future: i32s %8 = previous: i32s %12;
                goto lb8;
                lb10:
                [i32s %1] i32s %9 = phi(set [%11, bb4][%7, bb2]);
                [i32s %0] i32s %10 = phi(set [%12, bb4][%8, bb2]);
                exit num? 1;
            }
        }
    }
}
: