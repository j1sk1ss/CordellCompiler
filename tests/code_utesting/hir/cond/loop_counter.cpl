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
                lb9:
                goto lb10;
                lb10:
                {
                }
                i64t %1 = i64s %0 - cnst? 1;
                i64s %0 = i64t %1;
                if i64s %0, goto lb9, else goto lb11;
                lb11:
            }
        }
    }
}
/: