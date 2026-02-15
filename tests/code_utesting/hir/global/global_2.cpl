{
    glob arr a[10, i32] = {0};
    start() {
        a[0] = 1;
    }
}

: OUTPUT
{
    arrg %0 = arr_alloc(num? 10);
    start {
        {
            {
                i32t* %1 = &(arrg %0);
                i64t %2 = num? 0 * cnst? 4;
                i32t* %4 = i64t %2 as *ptr;
                i32t* %3 = i32t* %1 + i32t* %4;
                i32t %5 = num? 1 as i32;
                *(i32t* %3) = i32t %5;
            }
        }
    }
}
: