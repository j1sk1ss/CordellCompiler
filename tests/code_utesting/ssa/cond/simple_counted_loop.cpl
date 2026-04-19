start() {
    @[counter(10)] loop {
    }
}

:/ OUTPUT
{
    start {
        {
            {
                i64s %0 = alloc(8);
                i64s %2 = cnst? 10;
                future: i64s %3 = previous: i64s %2;
            }
            lb12:
            [i64s %0] i64s %3 = phi(set [%2, bb6][%4, bb3]);
            goto lb13;
            lb13:
            {
                i64t %1 = i64s %3 - cnst? 1;
                i64s %4 = i64t %1;
                future: i64s %3 = previous: i64s %4;
                if i64s %4, goto lb12, else goto lb14;
            [unused] }
            lb14:
        }
    }
}
/: