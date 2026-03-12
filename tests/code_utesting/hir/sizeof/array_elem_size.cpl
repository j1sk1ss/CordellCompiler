start() {
    arr a[10, ptr i32];
    i32 b = @[sizeof]a;
    i32 c = @[sizeof]a[0];
    i32 d = @[sizeof]a[0][0];
}

: OUTPUT
{
    start {
        {
            {
                arrs %0 = arr_alloc(num? 10);
                i32s %1 = alloc(8);
                [unused] use arrs %0;
                i32t %4 = cnst? 80 as i32;
                i32s %1 = i32t %4;
                i32s %2 = alloc(8);
                [unused] i32t** %5 = &(arrs %0);
                [unused] i64t %6 = num? 0 * cnst? 8;
                [unused] i32t** %8 = i64t %6 as *ptr;
                [unused] i32t** %7 = i32t** %5 + i32t** %8;
                [unused] i32t* %9 = *(i32t** %7);
                [unused] use i32t* %9;
                i32t %10 = cnst? 8 as i32;
                i32s %2 = i32t %10;
                i32s %3 = alloc(8);
                [unused] i32t** %11 = &(arrs %0);
                [unused] i64t %12 = num? 0 * cnst? 8;
                [unused] i32t** %14 = i64t %12 as *ptr;
                [unused] i32t** %13 = i32t** %11 + i32t** %14;
                [unused] i32t* %15 = *(i32t** %13);
                [unused] i32t* %16 = i32t* %15;
                [unused] i64t %17 = num? 0 * cnst? 4;
                [unused] i32t* %19 = i64t %17 as *ptr;
                [unused] i32t* %18 = i32t* %16 + i32t* %19;
                [unused] i32t %20 = *(i32t* %18);
                [unused] use i32t %20;
                i32t %21 = cnst? 4 as i32;
                i32s %3 = i32t %21;
            }
        }
    }
}
: