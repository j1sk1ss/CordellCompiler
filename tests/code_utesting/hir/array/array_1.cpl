start() {
    arr a[10, i32] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    a[0] = 0 as i32;
    i32 b = a[0];
    i32 c = dref a;
}

:/ OUTPUT
{
    start {
        {
            {
                arrs %0 = arr_alloc(i8n 10);
                i32t %3 = i8n 0 as i32;
                i32t* %4 = &(arrs %0);
                i8t %5 = i8n 0 * csu8 4;
                i32t* %7 = i8t %5 as *ptr;
                i32t* %6 = i32t* %4 + i32t* %7;
                *(i32t* %6) = i32t %3;
                i32s %1 = alloc(8);
                i32t* %8 = &(arrs %0);
                i8t %9 = i8n 0 * csu8 4;
                i32t* %11 = i8t %9 as *ptr;
                i32t* %10 = i32t* %8 + i32t* %11;
                i32t %12 = *(i32t* %10);
                i32s %1 = i32t %12;
                i32s %2 = alloc(8);
                i32t %13 = *(arrs %0);
                i32s %2 = i32t %13;
            }
        }
    }
}
/: