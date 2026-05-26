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
            i32t %0 = i8n 100 as i32;
            return i32t %0;
        }
    }
    fn _main()
    {
        {
            i32t %0 = i8n 100 as i32;
            lb21:
            [unused] getter0();
        }
    }
}
/: