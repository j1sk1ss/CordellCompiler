start() {
    i32 a;
    i32 b = dref ref a;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i32s %0 = alloc;
            i32s %1 = alloc;
            i32t* %2 = &(i32s %0);
            i32t %3 = *(i32t* %2);
            i32s %1 = i32t %3;
        }
    }
}
/: