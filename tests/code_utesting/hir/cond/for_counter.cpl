start() {
    i32 a = 10;
    i32 b = 1;
    @[counter(a, b)] loop {
    }

    @[counter(10)] loop {
    }

    @[counter(10, 1)] loop {
    }
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i32s %0 = alloc;
            i32t %2 = i8n 10 as i32;
            i32s %0 = i32t %2;
            i32s %1 = alloc;
            i32t %3 = i8n 1 as i32;
            i32s %1 = i32t %3;
            lb19:
            goto lb20;
            lb20:
            {
            }
            i32t %4 = i32s %0 - i32s %1;
            i32s %0 = i32t %4;
            if i32s %0, goto lb19, else goto lb21;
            lb21:
            i64s %5 = alloc;
            i64s %5 = csu8 10;
            lb31:
            goto lb32;
            lb32:
            {
            }
            i64t %6 = i64s %5 - csu8 1;
            i64s %5 = i64t %6;
            if i64s %5, goto lb31, else goto lb33;
            lb33:
            i64s %7 = alloc;
            i64s %7 = csu8 10;
            lb45:
            goto lb46;
            lb46:
            {
            }
            i64t %8 = i64s %7 - csu8 1;
            i64s %7 = i64t %8;
            if i64s %7, goto lb45, else goto lb47;
            lb47:
        }
    }
}
/: