{
    start() {
        arr a[10, i32] = {1,2,3,4,5,6,7,8,9,10};
        a[0] = 0;
        i32 b = a[0];
        i32 c = dref a;
    }
}

: OUTPUT
{
    start {
        {
            {
                use num? 1;
                use num? 2;
                use num? 3;
                use num? 4;
                use num? 5;
                use num? 6;
                use num? 7;
                use num? 8;
                use num? 9;
                use num? 10;
                arrs %0 = alloc(num? 10);
                u64t %3 = &(arrs %0);
                i64t %4 = num? 0 * cnst? 4;
                u64t %6 = i64t %4 as u64;
                u64t %5 = u64t %3 + u64t %6;
                i32t %7 = num? 0 as i32;
                *(u64t %5) = i32t %7;
                i32s %1 = alloc(8);
                u64t %8 = &(arrs %0);
                i64t %9 = num? 0 * cnst? 4;
                u64t %11 = i64t %9 as u64;
                u64t %10 = u64t %8 + u64t %11;
                i32t %12 = *(u64t %10);
                i32s %1 = i32t %12;
                i32s %2 = alloc(8);
                i32t %13 = *(arrs %0);
                i32s %2 = i32t %13;
            }
        }
    }
}
: