@[like_c]
container s {
    i8  a;
    i16 b;
    i32 c;
    i64 d;
}

start() {
    s storage;
    storage.a = 0;
    storage.b = 0;
    storage.c = 0;
    storage.d = 0;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            u8s %4 = arr_alloc(csu8 16);
            u8t* %5 = &(u8s %4);
            u8t* %6 = u8t* %5 + csu8 0;
            i8t* %7 = u8t* %6 as *ptr;
            *(i8t* %7) = i8n 0;
            u8t* %8 = &(u8s %4);
            u8t* %9 = u8t* %8 + csu8 2;
            i16t* %10 = u8t* %9 as *ptr;
            i16t %11 = i8n 0 as i16;
            *(i16t* %10) = i16t %11;
            u8t* %12 = &(u8s %4);
            u8t* %13 = u8t* %12 + csu8 4;
            i32t* %14 = u8t* %13 as *ptr;
            i32t %15 = i8n 0 as i32;
            *(i32t* %14) = i32t %15;
            u8t* %16 = &(u8s %4);
            u8t* %17 = u8t* %16 + csu8 8;
            i64t* %18 = u8t* %17 as *ptr;
            i64t %19 = i8n 0 as i64;
            *(i64t* %18) = i64t %19;
        }
    }
}
/: