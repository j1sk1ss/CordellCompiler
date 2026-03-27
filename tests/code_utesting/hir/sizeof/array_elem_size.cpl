start() {
    arr a[10, ptr i32];
    i32 b = sizeof(a);
    i32 c = sizeof(a[0]);
    i32 d = sizeof(a[0][0]);
}

: OUTPUT
{
    start {
        {
            {
                arrs %0 = arr_alloc(num? 10);
                i32s %1 = alloc(8);
                i32t %4 = cnst? 80 as i32;
                i32s %1 = i32t %4;
                i32s %2 = alloc(8);
                i32t %10 = cnst? 8 as i32;
                i32s %2 = i32t %10;
                i32s %3 = alloc(8);
                i32t %21 = cnst? 4 as i32;
                i32s %3 = i32t %21;
            }
        }
    }
}
: