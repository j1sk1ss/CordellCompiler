start() {
    i32 a = (10 + 11 * 12) as i32;
    i32 b = (10 - 1) as i32;
    a += b += a; 
}

: OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i64t %3 = num? 11 * num? 12;
                u64t %5 = num? 10 as u64;
                u64t %6 = i64t %3 as u64;
                u64t %4 = u64t %5 + u64t %6;
                i32t %2 = u64t %4 as i32;
                i32s %0 = i32t %2;
                i32s %1 = alloc(8);
                i64t %8 = num? 10 - num? 1;
                i32t %7 = i64t %8 as i32;
                i32s %1 = i32t %7;
                i32t %9 = i32s %1 + i32s %0;
                i32s %1 = i32t %9;
                i32t %10 = i32s %0 + i32s %1;
                i32s %0 = i32t %10;
            }
        }
    }
}
: