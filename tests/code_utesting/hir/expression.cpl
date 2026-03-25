start() {
    i32 a = 100;
    i32 b = 200;
    i32 c = (a + b) * a;
    i32 d = (a + b) + a;
}

: OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i32t %4 = num? 100 as i32;
                i32s %0 = i32t %4;
                i32s %1 = alloc(8);
                i32t %5 = num? 200 as i32;
                i32s %1 = i32t %5;
                i32s %2 = alloc(8);
                i32t %6 = i32s %0 + i32s %1;
                i32t %7 = i32t %6 * i32s %0;
                i32s %2 = i32t %7;
                i32s %3 = alloc(8);
                i32t %8 = i32s %0 + i32s %1;
                i32t %9 = i32t %8 + i32s %0;
                i32s %3 = i32t %9;
            }
        }
    }
}
: