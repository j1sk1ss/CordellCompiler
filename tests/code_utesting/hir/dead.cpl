start() {
    u8 a = 1;
    exit a;
    u8 b = 1;
    exit b;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            u8s %0 = alloc;
            u8t %2 = i8n 1 as u8;
            u8s %0 = u8t %2;
            exit u8s %0;
            u8s %1 = alloc;
            u8t %3 = i8n 1 as u8;
            u8s %1 = u8t %3;
            exit u8s %1;
        }
    }
}
/: