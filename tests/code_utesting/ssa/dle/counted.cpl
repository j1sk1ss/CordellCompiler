start() {
    @[counter(100)] loop {
    }
}

:/ OUTPUT
{
    start {
        {
            {
                i64s %0 = alloc(8);
                i64s %2 = csu8 100;
                future: i64s %3 = previous: i64s %2;
                lb9:
                [i64s %0] i64s %3 = phi(set [%4, bb3][%2, bb5]);
                goto lb10;
                lb10:
                {
                }
                i64t %1 = i64s %3 - csu8 1;
                i64s %4 = i64t %1;
                future: i64s %3 = previous: i64s %4;
                if i64s %4, goto lb9, else goto lb11;
                lb11:
            }
        }
    }
}
/: