@[align(1)]
container a {
    i64 kek;
    i32 f;
}

@[align(1)]
container b {
    i8    lol;
    ptr a k;
}

start() {
    b nd;
    i32 a = nd.k.f;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            u8s %4 = arr_alloc(csu8 9);
            i32s %5 = alloc;
            u8t* %6 = &(u8s %4);
            u8t %7 = u8t* %6 + csu8 1;
            i8t %8 = *(u8t %7);
            i8t* %9 = &(i8t %8);
            u8t %10 = i8t* %9 + csu8 8;
            i32t %11 = *(u8t %10);
            i32s %5 = i32t %11;
        }
    }
}
/: