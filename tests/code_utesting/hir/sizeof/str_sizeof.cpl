start() {
    arr a[0, i8] = "Hello world1";
    arr b[0, i8] = "world2";
    i32 c = sizeof(a) as i32;
    i32 d = sizeof(b) as i32;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            strs %0 = str_alloc(Hello world1);
            strs %1 = str_alloc(world2);
            i32s %2 = alloc;
            i32t %4 = csu8 13 as i32;
            i32s %2 = i32t %4;
            i32s %3 = alloc;
            i32t %5 = csu8 7 as i32;
            i32s %3 = i32t %5;
        }
    }
}
/: