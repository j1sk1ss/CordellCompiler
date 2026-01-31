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
                i8s* %0 = alloc(8);
                i8t* %2 = Hello, World! as u64;
                i8s* %0 = i8t* %2;
                strs* %1 = alloc(8);
                strs* %1 = Hello, User!;
                u64t %3 = i8s* %0;
                i64t %4 = num? 0 * cnst? 1;
                u64t %6 = i64t %4 as u64;
                u64t %5 = u64t %3 + u64t %6;
                i8t %7 = num? 111 as i8;
                *(u64t %5) = i8t %7;
                u64t %8 = &(strs %1);
                i64t %9 = num? 1 * cnst? 1;
                u64t %11 = i64t %9 as u64;
                u64t %10 = u64t %8 + u64t %11;
                i8t %12 = num? 103 as i8;
                *(u64t %10) = i8t %12;
            }
        }
    }
}
: