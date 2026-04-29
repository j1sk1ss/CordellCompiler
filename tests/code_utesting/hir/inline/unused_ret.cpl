function getter() -> i32 {
    return 100;
}

start() {
    getter();
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
                lb20:
                [unused] getter0();
            }
        }
    }
}
/: