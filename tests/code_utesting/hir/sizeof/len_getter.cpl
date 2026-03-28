start() {
    arr a[10, i16];
    i32 a_len = (sizeof(a) / sizeof(a[0])) as i32;
}

:/ OUTPUT
{
    start {
        {
            {
                arrs %0 = arr_alloc(num? 10);
                i32s %1 = alloc(8);
                i64t %8 = cnst? 20 / cnst? 2;
                i32t %2 = i64t %8 as i32;
                i32s %1 = i32t %2;
            }
        }
    }
}
/:
