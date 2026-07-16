function free(ptr i0 p);
container std {
    function free(ptr i8 p) {
        free(p as ptr i0);
    }
}

start() {
    std::free(0 as ptr i8);
}

:/ OUTPUT
{
    fn free1(i8* p)
    {
        i8s* %1 = alloc;
        i8s* %1 = load_arg();
        {
            i0t* %2 = i8s* %1 as *ptr;
            use i0t* %2;
            free0(i0t* %2);
        }
    }
    fn _main()
    {
        {
            i8t* %3 = i8n 0 as *ptr;
            use i8t* %3;
            free1(i8t* %3);
        }
    }
}
/: