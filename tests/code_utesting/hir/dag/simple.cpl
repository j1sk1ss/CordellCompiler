{
    start() {
        i32 a = 10;
        i32 b = 10;
        i32 c = a + b;
        i32 d = a + b + c;
    }
}

: OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i32t %4 = num? 10 as i32;
                i32s %9 = i32t %4;
                i32s %1 = alloc(8);
                [unused] i32t %5 = num? 10 as i32;
                i32s %10 = i32t %4;
                i32s %2 = alloc(8);
                i32t %6 = i32s %9 + i32s %9;
                i32s %11 = i32t %6;
                i32s %3 = alloc(8);
                [unused] i32t %7 = i32s %9 + i32s %9;
                i32t %8 = i32t %6 + i32s %11;
                i32s %12 = i32t %8;
            }
        }
    }
}
: