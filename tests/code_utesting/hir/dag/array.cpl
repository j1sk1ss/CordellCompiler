{
    start() {
        arr a[10, i32] = {
            10, 11, 12, 13, 14 + 15, 14 + 15, 10, 10, 10, 10
        };
    }
}

: OUTPUT
{
    start {
        {
            {
                use num? 10;
                use num? 11;
                use num? 12;
                use num? 13;
                i64t %1 = num? 14 + num? 15;
                use i64t %1;
                [unused] i64t %2 = num? 14 + num? 15;
                use i64t %1;
                use num? 10;
                use num? 10;
                use num? 10;
                use num? 10;
                arrs %3 = arr_alloc(num? 10);
            }
        }
    }
}
: