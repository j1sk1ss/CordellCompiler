{
    start() {
        i32 a = 0;
        loop {
            a = 1;
            break;
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
                i32s %4 = i32t %2;
                lb8:
                goto lb9;
                lb9:
                {
                    i32t %3 = num? 1 as i32;
                    i32s %5 = i32t %3;
                    // break;
                    goto lb10;
                }
                goto lb8;
                lb10:
                i32s %1 = alloc(8);
                i32s %6 = i32s %5;
            }
        }
    }
}
: