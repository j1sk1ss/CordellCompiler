{
    start() {
        i32 a = 0;
        if 1; {
            a = 1;
        }
        else {
            a = 2;
        }
        i32 b = a;
    }
}

: OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i32t %2 = num? 0 as i32;
                i32s %5 = i32t %2;
                if num? 1, goto lb8, else goto lb9;
                lb8:
                {
                    i32t %3 = num? 1 as i32;
                    i32s %9 = i32t %3;
                }
                future: i32s %7 = previous: i32s %9;
                goto lb10;
                lb9:
                {
                    i32t %4 = num? 2 as i32;
                    i32s %6 = i32t %4;
                }
                future: i32s %7 = previous: i32s %6;
                goto lb10;
                lb10:
                [i32s %0] i32s %7 = phi(set [%6, bb13][%9, bb12]);
                i32s %1 = alloc(8);
                i32s %8 = i32s %7;
            }
        }
    }
}
: