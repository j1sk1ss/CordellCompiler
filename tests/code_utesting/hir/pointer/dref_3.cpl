start() {
    ptr i8 a;
    ptr ptr i8 b;
    dref a = 1 as i8;
    dref b = 1;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i8s* %0 = alloc;
            i8s** %1 = alloc;
            i8t %2 = i8n 1 as i8;
            *(i8s* %0) = i8t %2;
            i8t* %3 = i8n 1 as *ptr;
            *(i8s** %1) = i8t* %3;
        }
    }
}
/: