start() {
    arr a[10, i32] = {
        10, 11, 12, 13, 14 + 15, 14 + 15, 10, 10, 10, 10
    };
}

:/ OUTPUT
{
    start {
        {
            {
                i8t %1 = i8n 14 + i8n 15;
                use i8t %1;
                [unused] i8t %2 = i8n 14 + i8n 15;
                use i8t %1;
                arrs %3 = arr_alloc(i8n 10);
            }
        }
    }
}
/: