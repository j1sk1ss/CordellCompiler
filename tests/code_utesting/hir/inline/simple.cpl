function getter() -> i32 {
    return 100;
}

start() {
    exit getter() as u8;
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
            i32t %2 = i32t %0;
            goto lb23;
            lb23:
            [unused] i32t %2 = getter0();
            u8t %1 = i32t %2 as u8;
            exit u8t %1;
        }
    }
}
/: