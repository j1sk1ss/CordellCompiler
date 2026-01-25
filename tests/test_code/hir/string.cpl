{
    start() {
        ptr i8 msg = "Hello, World!";
        str ans = "Hello, User!";

        msg[0] = 'o';
        ans[1] = 'g';
    }
}

: OUTPUT
{
    start {
        {
            {
                u64s %0 = alloc(8);
                u64t %2 = Hello, World! as u64;
                u64s %0 = u64t %2;
                u64s %0 = u64t %2;
                strs %1 = alloc(Hello, User!);
                u64t %3 = num? 111 as u64;
                u64t %4 = u64s %0;
                i64t %5 = num? 0 * cnst? 1;
                u64t %7 = i64t %5 as u64;
                u64t %6 = u64t %4 + u64t %7;
                *(u64t %6) = u64t %3;
                i8t %8 = num? 103 as i8;
                u64t %9 = &(strs %1);
                i64t %10 = num? 1 * cnst? 1;
                u64t %12 = i64t %10 as u64;
                u64t %11 = u64t %9 + u64t %12;
                *(u64t %11) = i8t %8;
            }
        }
    }
}
: