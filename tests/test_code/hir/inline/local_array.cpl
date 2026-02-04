{
    function foo() {
        arr a[10, i32] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        return a[0];
    }

    start() {
        exit foo();
    }
}

: OUTPUT
{
    fn foo0()
    {
        {
            use num? 0;
            use num? 1;
            use num? 2;
            use num? 3;
            use num? 4;
            use num? 5;
            use num? 6;
            use num? 7;
            use num? 8;
            use num? 9;
            arrs* %0 = arr_alloc(num? 10);
            u64t %1 = &(arrs* %0);
            i64t %2 = num? 0 * cnst? 4;
            u64t %4 = i64t %2 as u64;
            u64t %3 = u64t %1 + u64t %4;
            i32t %5 = *(u64t %3);
            return i32t %5;
        }
    }
    start {
        {
            {
                use num? 0;
                use num? 1;
                use num? 2;
                use num? 3;
                use num? 4;
                use num? 5;
                use num? 6;
                use num? 7;
                use num? 8;
                use num? 9;
                arrs* %0 = arr_alloc(num? 10);
                u64t %1 = &(arrs* %0);
                i64t %2 = num? 0 * cnst? 4;
                u64t %4 = i64t %2 as u64;
                u64t %3 = u64t %1 + u64t %4;
                i32t %5 = *(u64t %3);
                i64t %6 = i32t %5;
                [unused] i64t %6 = call foo0(), argc args();
                exit i64t %6;
            }
        }
    }
}
: