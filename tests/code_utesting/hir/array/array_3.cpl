{
    start() {
        ptr ptr i32 a;
        ptr i32 b = a[0];
        i32 c = b[0];
    }
}

: OUTPUT
{
    start {
        {
            {
                i32s** %0 = alloc(8);
                i32s* %1 = alloc(8);
                i32t** %3 = i32s** %0;
                i64t %4 = num? 0 * cnst? 4;
                i32t** %6 = i64t %4 as *ptr;
                i32t** %5 = i32t** %3 + i32t** %6;
                i32t* %7 = *(i32t** %5);
                i32s* %1 = i32t* %7;
                i32s %2 = alloc(8);
                i32t* %8 = i32s* %1;
                i64t %9 = num? 0 * cnst? 4;
                i32t* %11 = i64t %9 as *ptr;
                i32t* %10 = i32t* %8 + i32t* %11;
                i32t %12 = *(i32t* %10);
                i32s %2 = i32t %12;
            }
        }
    }
}
: