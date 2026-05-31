container node {
    i32 a;
    i32 b;
}

start() {
    arr array[10, node];
    array[0].a = 0;
    array[0].b = 0;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            arrs %2 = arr_alloc(csi8 160);
            i8t* %3 = &(arrs %2);
            i8t %4 = i8n 0 * csu8 16;
            i8t* %6 = i8t %4 as *ptr;
            i8t* %5 = i8t* %3 + i8t* %6;
            u8t* %7 = i8t* %5 + csu8 0;
            i32t* %8 = u8t* %7;
            i32t %9 = i8n 0 as i32;
            *(i32t* %8) = i32t %9;
            i8t* %10 = &(arrs %2);
            i8t %11 = i8n 0 * csu8 16;
            i8t* %13 = i8t %11 as *ptr;
            i8t* %12 = i8t* %10 + i8t* %13;
            u8t* %14 = i8t* %12 + csu8 8;
            i32t* %15 = u8t* %14;
            i32t %16 = i8n 0 as i32;
            *(i32t* %15) = i32t %16;
        }
    }
}
/: