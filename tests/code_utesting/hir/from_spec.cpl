start() {
    i32 a = 1;
    i32 b = 2;
    exit (a + b) as u8;
}

:/ OUTPUT
{
    fn _main0()
    {
        {
            i32s %0 = alloc;
            i32t %2 = i8n 1 as i32;
            i32s %0 = i32t %2;
            i32s %1 = alloc;
            i32t %3 = i8n 2 as i32;
            i32s %1 = i32t %3;
            i32t %5 = i32s %0 + i32s %1;
            u8t %4 = i32t %5 as u8;
            exit u8t %4;
        }
    }
}
/: