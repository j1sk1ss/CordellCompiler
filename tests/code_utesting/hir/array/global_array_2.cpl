glob arr a[10, i32];
start() {
    a[0] = 1 as i32;
}

:/ OUTPUT
{
    arrg %0 = arr_alloc(i8n 10);
    start {
        {
            {
                i32t %1 = i8n 1 as i32;
                i32t* %2 = &(arrg %0);
                i8t %3 = i8n 0 * csu8 4;
                i32t* %5 = i8t %3 as *ptr;
                i32t* %4 = i32t* %2 + i32t* %5;
                *(i32t* %4) = i32t %1;
            }
        }
    }
}
/: