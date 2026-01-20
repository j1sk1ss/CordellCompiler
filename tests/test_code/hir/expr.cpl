{
    start() {
        i32 a = 10 + 11 * 12;
        i32 b = 10 - 1;
        a += b += a; 
    }
}

: OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i64t %2 = num? 11 * num? 12;
                u64t %4 = num? 10 as u64;
                u64t %5 = i64t %2 as u64;
                u64t %3 = u64t %4 + u64t %5;
                i32t %6 = u64t %3 as i32;
                i32s %0 = i32t %6;
                i32s %1 = alloc(8);
                i64t %7 = num? 10 - num? 1;
                i32t %8 = i64t %7 as i32;
                i32s %1 = i32t %8;
                i32t %10 = i32s %0 + i32t %9;
                i32s %0 = i32t %10;
            }
        }
    }
}
: