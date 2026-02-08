{
    glob arr a[10, i32] = {0};
    start() {
        a[0] = 1;
    }
}

: OUTPUT
{
    arrg* %0 = arr_alloc(num? 10);
    start {
        {
            {
                u64t %1 = &(arrg* %0);
                i64t %2 = num? 0 * cnst? 4;
                u64t %4 = i64t %2 as u64;
                u64t %3 = u64t %1 + u64t %4;
                i32t %5 = num? 1 as i32;
                *(u64t %3) = i32t %5;
            }
        }
    }
}
: