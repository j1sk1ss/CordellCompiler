glob i32 a;
start() {
    i32 b = a + 1 as i32;
    a = b;
}

:/ OUTPUT
{
    start {
        {
            {
                i32s %1 = alloc(8);
                i32t %2 = i8n 1 as i32;
                i32t %3 = i32g %0 + i32t %2;
                i32s %1 = i32t %3;
                i32g %0 = i32s %1;
            }
        }
    }
}
/: