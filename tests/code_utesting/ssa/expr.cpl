start() {
    i32 a = 10 + 11 * 12;
    i32 b = 10 - 1;
    a += b += a; 
}

:/ OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i8t %2 = i8n 11 * i8n 12;
                i8t %3 = i8n 10 + i8t %2;
                i32t %4 = i8t %3 as i32;
                i32s %9 = i32t %4;
                i32s %1 = alloc(8);
                i8t %5 = i8n 10 - i8n 1;
                i32t %6 = i8t %5 as i32;
                i32s %10 = i32t %6;
                i32t %7 = i32s %10 + i32s %9;
                i32s %11 = i32t %7;
                i32t %8 = i32s %9 + i32s %11;
                i32s %12 = i32t %8;
            }
        }
    }
}
/: