function getter() -> i32 {
    return 100;
}

start() {
    exit getter();
}

:/ OUTPUT
{
    fn getter0() -> i32
    {
        {
            return i8n 100;
        }
    }
    start {
        {
            {
                i32t %0 = i8n 100;
                goto lb21;
                lb21:
                [unused] i32t %0 = getter0();
                exit i32t %0;
            }
        }
    }
}
/: