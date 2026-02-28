{
    start() {
        ptr i32 a = ref (10 + 10);
    }
}

: OUTPUT
{
    start {
        {
            {
                i32s* %0 = alloc(8);
                i64t %1 = num? 10 + num? 10;
                i64t* %2 = &(i64t %1);
                i32t* %3 = i64t* %2 as *ptr;
                i32s* %0 = i32t* %3;
            }
        }
    }
}
: