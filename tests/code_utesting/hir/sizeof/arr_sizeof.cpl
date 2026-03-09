{
    start() {
        arr a[10, i32];
        arr b[15, i8];
        i32 c = @[sizeof] a;
        i32 d = @[sizeof] b;
    }
}

: OUTPUT
{
    start {
        {
            {
                arrs %0 = arr_alloc(num? 10);
                arrs %1 = arr_alloc(num? 15);
                i32s %2 = alloc(8);
                i32t %4 = cnst? 40 as i32;
                i32s %2 = i32t %4;
                i32s %3 = alloc(8);
                i32t %5 = cnst? 15 as i32;
                i32s %3 = i32t %5;
            }
        }
    }
}
: