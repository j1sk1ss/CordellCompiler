{
    start() {
        i32 d = 0;
        loop {
            i32 c = 10 + 10;
            d += c;
        }
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
                [i32s %1] i32s %7 = phi(set [%9, bb13][%1, bb15]);
                [i32s %0] i32s %8 = phi(set [%6, bb15][%10, bb13]);
                goto lb9;
                lb9:
                {
                    i32s %7 = alloc(8);
                    i32s %9 = i32t %4;
                    i32t %5 = i32s %8 + i32s %9;
                    i32s %10 = i32t %5;
                }
                future: i32s %7 = previous: i32s %9;
                future: i32s %8 = previous: i32s %10;
                goto lb8;
                lb10:
            }
        }
    }
}
: