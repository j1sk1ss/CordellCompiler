{
    start() {
        arr a[10, i16];
        i32 a_len = @[sizeof]a / @[sizeof]a[0];
    }
}

: OUTPUT
{
    start {
        {
            {
                arrs %0 = arr_alloc(num? 10);
                i32s %1 = alloc(8);
                [unused] use arrs %0;
                [unused] i16t* %2 = &(arrs %0);
                [unused] i64t %3 = num? 0 * cnst? 2;
                [unused] i16t* %5 = i64t %3 as *ptr;
                [unused] i16t* %4 = i16t* %2 + i16t* %5;
                [unused] i16t %6 = *(i16t* %4);
                [unused] use i16t %6;
                i64t %7 = cnst? 20 / cnst? 2;
                i32t %8 = i64t %7 as i32;
                i32s %1 = i32t %8;
            }
        }
    }
}
: