start() {
    i64 a;
    u64 b;
    i32 c = sizeof(a) as i32;
    i32 d = sizeof(b) as i32;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i64s %0 = alloc;
            u64s %1 = alloc;
            i32s %2 = alloc;
            i32t %4 = csu8 8 as i32;
            i32s %2 = i32t %4;
            i32s %3 = alloc;
            i32t %5 = csu8 8 as i32;
            i32s %3 = i32t %5;
        }
    }
}
/: