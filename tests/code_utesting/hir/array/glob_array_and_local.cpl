{
    glob arr a[1, i32];
    start() {
        arr b[1, ptr i32] = { ref a };
    }
}

: OUTPUT
{
    arrg %0 = arr_alloc(num? 1);
    start {
        {
            {
                i32t* %2 = &(arrg %0);
                use i32t* %2;
                arrs %1 = arr_alloc(num? 1);
            }
        }
    }
}
: