start() {
    ptr i32 a = ref (10 + 10);
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i32s* %0 = alloc;
            i8t %1 = i8n 10 + i8n 10;
            i8t* %2 = &(i8t %1);
            i32t* %3 = i8t* %2 as *ptr;
            i32s* %0 = i32t* %3;
        }
    }
}
/: