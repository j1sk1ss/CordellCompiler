start() {
    i32 a = 10;
    @[counter(10)] loop {
    }
    i32 b = 10;
}

:/ OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc;
                i32t %2 = i8n 10 as i32;
                i32s %6 = i32t %2;
                i64s %3 = alloc;
                i64s %7 = csu8 10;
                [SSA] future: i64s %8 <<== previous: i64s %7;
                i32t %5 = i8n 10 as i32;
                i32s %10 = i32t %5;
                lb14:
                [base: i64s %3] i64s %8 = phi(set [%9, bb3][%7, bb5]);
                goto lb15;
                lb15:
                {
                }
                i64t %4 = i64s %8 - csu8 1;
                i64s %9 = i64t %4;
                [SSA] future: i64s %8 <<== previous: i64s %9;
                if i64s %9, goto lb14, else goto lb16;
                lb16:
                i32s %1 = alloc;
            }
        }
    }
}
/: