{
    start() {
        i32 d = 0;
        while 1; {
            i32 c = 10 + 10;
            d += c;
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
                future: i32s %8 = previous: i32s %1;
                future: i32s %7 = previous: i32s %6;
                i64t %3 = num? 10 + num? 10;
                i32t %4 = i64t %3 as i32;
                lb8:
                [i32s %1] i32s %7 = phi(set [{X}][{X}]);
                [i32s %0] i32s %8 = phi(set [{X}][{X}]);
                if num? 1, goto lb9, else goto lb10;
                lb9:
                {
                    i32s %8 = alloc(8);
                    i32s %9 = i32t %4;
                    i32t %5 = i32s %7 + i32s %9;
                    i32s %10 = i32t %5;
                }
                future: i32s %7 = previous: i32s %10;
                future: i32s %8 = previous: i32s %9;
                goto lb8;
                lb10:
                exit num? 1;
            }
        }
    }
}
: