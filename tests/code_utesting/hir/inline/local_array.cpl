function foo() {
    arr a[10, i32] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    return a[0];
}

start() {
    exit foo();
}

:/ OUTPUT
{
    fn foo0()
    {
        {
            arrs %0 = arr_alloc(i8n 10);
            i32t* %1 = &(arrs %0);
            i8t %2 = i8n 0 * csu8 4;
            i32t* %4 = i8t %2 as *ptr;
            i32t* %3 = i32t* %1 + i32t* %4;
            i32t %5 = *(i32t* %3);
            return i32t %5;
        }
    }
    [unused]
    start {
        {
            {
                arrs %0 = arr_alloc(i8n 10);
                i32t* %1 = &(arrs %0);
                i8t %2 = i8n 0 * csu8 4;
                i32t* %4 = i8t %2 as *ptr;
                i32t* %3 = i32t* %1 + i32t* %4;
                i32t %5 = *(i32t* %3);
                i64t %6 = i32t %5;
                [unused] i64t %6 = call foo0(), argc args();
                exit i64t %6;
            }
        }
    [unused] }
}
/: