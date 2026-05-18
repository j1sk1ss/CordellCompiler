start() {
    @[align(64)] i8 a;
    i32 b = sizeof(a) as i32;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i8s %0 = alloc;
            i32s %1 = alloc;
            i32t %2 = csu8 64 as i32;
            i32s %1 = i32t %2;
        }
    }
}
/: