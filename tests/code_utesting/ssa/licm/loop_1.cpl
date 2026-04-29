start() {
    i32 d = 0;
    loop {
        i32 c = 10 + 10;
        d += c;
    }
}

:/ OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc;
                i32t %2 = i8n 0 as i32;
                i32s %6 = i32t %2;
                [SSA] future: i32s %7 <<== previous: i32s %6;
                i8t %3 = i8n 10 + i8n 10;
                i32t %4 = i8t %3 as i32;
                i32s %8 = i32t %4;
                lb12:
                [base: i32s %0] i32s %7 = phi(set [%6, bb5][%9, bb3]);
                [base: i32s %1] i32s %10 = phi(set [%8, bb3]);
                goto lb13;
                lb13:
                {
                    i32s %1 = alloc;
                    i32t %5 = i32s %7 + i32s %8;
                    i32s %9 = i32t %5;
                }
                [SSA] future: i32s %7 <<== previous: i32s %9;
                [SSA] future: i32s %10 <<== previous: i32s %8;
                goto lb12;
                lb14:
            [unused] }
        [unused] }
    [unused] }
}
/: