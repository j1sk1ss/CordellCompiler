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
    nd.k.f = 0;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            u8s %4 = arr_alloc(csu8 9);
            u8t* %5 = &(u8s %4);
            u8t %6 = u8t* %5 + csu8 1;
            i8t %7 = *(u8t %6);
            i8t* %8 = &(i8t %7);
            u8t %9 = i8t* %8 + csu8 8;
            *(u8t %9) = i8n 0;
        }
    }
}
/: