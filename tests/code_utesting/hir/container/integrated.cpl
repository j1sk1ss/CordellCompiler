container a {
    i32 a;
    i32 b;
}

container b {
    a     stack;
    ptr a pointer;
}

start() {
    b s;
    s.stack.a = 0;
    s.pointer.a = 0;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            u8s %4 = arr_alloc(csu8 24);
            u8t* %5 = &(u8s %4);
            u8t* %6 = u8t* %5 + csu8 0;
            u8t* %7 = u8t* %6 + csu8 0;
            i32t* %8 = u8t* %7;
            i32t %9 = i8n 0 as i32;
            *(i32t* %8) = i32t %9;
            u8t* %10 = &(u8s %4);
            u8t* %11 = u8t* %10 + csu8 16;
            u8t** %13 = u8t* %11 as *ptr;
            u8t* %12 = *(u8t** %13);
            u8t* %14 = u8t* %12 + csu8 0;
            i32t* %15 = u8t* %14;
            i32t %16 = i8n 0 as i32;
            *(i32t* %15) = i32t %16;
        }
    }
}
/: