start() {
    i32 a = (10 + 11 * 12) as i32;
    i32 b = (10 - 1) as i32;
    a += b += a; 
}

:/ OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i8t %3 = i8n 11 * i8n 12;
                i8t %4 = i8n 10 + i8t %3;
                i32t %2 = i8t %4 as i32;
                i32s %0 = i32t %2;
                i32s %1 = alloc(8);
                i8t %6 = i8n 10 - i8n 1;
                i32t %5 = i8t %6 as i32;
                i32s %1 = i32t %5;
                i32t %7 = i32s %1 + i32s %0;
                i32s %1 = i32t %7;
                i32t %8 = i32s %0 + i32s %1;
                i32s %0 = i32t %8;
            }
        }
    }
}
/: