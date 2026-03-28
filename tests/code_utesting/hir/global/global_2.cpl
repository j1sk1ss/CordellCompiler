glob arr a[10, i32] = {0};
start() {
    a[0] = 1 as i32;
}

:/ OUTPUT
{
    arrg %0 = arr_alloc(num? 10);
    start {
        {
            {
                i32t %1 = num? 1 as i32;
                i32t* %2 = &(arrg %0);
                i64t %3 = num? 0 * cnst? 4;
                i32t* %5 = i64t %3 as *ptr;
                i32t* %4 = i32t* %2 + i32t* %5;
                *(i32t* %4) = i32t %1;
            }
        }
    }
}
/:
