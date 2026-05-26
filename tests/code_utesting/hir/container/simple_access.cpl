@[align(1)]
container node {
    i32     a;
    ptr i32 b;
}

start() {
    node nd;
    i32 a = nd.a;
    ptr i32 b = nd.b;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            u8s %2 = arr_alloc(csu8 12);
            i32s %3 = alloc;
            u8t* %5 = &(u8s %2);
            u8t %6 = u8t* %5 + csu8 0;
            i32t %7 = *(u8t %6);
            i32s %3 = i32t %7;
            i32s* %4 = alloc;
            u8t* %8 = &(u8s %2);
            u8t %9 = u8t* %8 + csu8 4;
            i32t* %10 = *(u8t %9);
            i32s* %4 = i32t* %10;
        }
    }
}
/: