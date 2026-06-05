@[union]
container node {
    i8  a;
    i16 b;
    i32 c;
    i64 d;
}

container string {
    node nd1;
    node nd2;
}

start() {
    node nd;
    nd.a = 0;
    nd.d = 0;

    string obj;
    obj.nd1.a = 0;
    obj.nd2.b = 0;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            u8s %6 = arr_alloc(csu8 8);
            u8t* %8 = &(u8s %6);
            u8t* %9 = u8t* %8 + csu8 0;
            i8t* %10 = u8t* %9 as *ptr;
            *(i8t* %10) = i8n 0;
            u8t* %11 = &(u8s %6);
            u8t* %12 = u8t* %11 + csu8 0;
            i64t* %13 = u8t* %12 as *ptr;
            i64t %14 = i8n 0 as i64;
            *(i64t* %13) = i64t %14;
            u8s %7 = arr_alloc(csu8 16);
            u8t* %15 = &(u8s %7);
            u8t* %16 = u8t* %15 + csu8 0;
            u8t* %17 = u8t* %16 + csu8 0;
            i8t* %18 = u8t* %17 as *ptr;
            *(i8t* %18) = i8n 0;
            u8t* %19 = &(u8s %7);
            u8t* %20 = u8t* %19 + csu8 8;
            u8t* %21 = u8t* %20 + csu8 0;
            i16t* %22 = u8t* %21 as *ptr;
            i16t %23 = i8n 0 as i16;
            *(i16t* %22) = i16t %23;
        }
    }
}
/: