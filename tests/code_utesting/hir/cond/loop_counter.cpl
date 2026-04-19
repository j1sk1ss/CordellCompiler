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
                i64s %0 = cnst? 10;
                lb12:
                goto lb13;
                lb13:
                {
                    i64t %1 = i64s %0 - cnst? 1;
                    i64s %0 = i64t %1;
                    if i64s %0, goto lb12, else goto lb14;
                }
                lb14:
            }
        }
    }
}
/: