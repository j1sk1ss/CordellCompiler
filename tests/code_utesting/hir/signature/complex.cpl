function strlen(ptr i8 s) -> i32 {
    i32 l = 0;
    return l;
}

start() {
    i32 a = strlen(ref "asd");
}

:/ OUTPUT
{
    fn strlen0(i8* s) -> i32
    {
        i8s* %0 = alloc;
        i8s* %0 = load_arg();
        {
            i32s %1 = alloc;
            i32t %3 = i8n 0 as i32;
            i32s %1 = i32t %3;
            return i32s %1;
        }
    }
    fn _main()
    {
        {
            i32s %2 = alloc;
            i8t* %4 = &(asd);
            use i8t* %4;
            i32t %5 = strlen0(i8t* %4);
            i32s %2 = i32t %5;
        }
    }
}
/: