: BUG :
container data {
    arr padding[10, i32];
    i32 a;
}

start() {
    data d;
    d.padding[0] = 1;
    d.a = 0;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            u8s %2 = arr_alloc(csu8 48);
            u8t* %3 = &(u8s %2);
            u8t* %4 = u8t* %3 + csu8 0;
            i8t %5 = *(u8t* %4);
            i8t %6 = i8t %5;
            i8t %7 = i8n 0 * csu8 1;
            i8t %8 = i8t %6 + i8t %7;
            i8t %9 = i8n 1 as i8;
            *(i8t %8) = i8t %9;
            u8t* %10 = &(u8s %2);
            u8t* %11 = u8t* %10 + csu8 8;
            i32t* %12 = u8t* %11;
            i32t %13 = i8n 0 as i32;
            *(i32t* %12) = i32t %13;
        }
    }
}
/: