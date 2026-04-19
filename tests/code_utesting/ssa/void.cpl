start() {
    ptr i0 a = 0x1;
    a = 0x2;
    a = 0x3;
}

:/ OUTPUT
{
    start {
        {
            {
                i0s* %0 = alloc(8);
                i0t* %1 = i8n 1 as *ptr;
                i0s* %4 = i0t* %1;
                i0t* %2 = i8n 2 as *ptr;
                i0s* %5 = i0t* %2;
                i0t* %3 = i8n 3 as *ptr;
                i0s* %6 = i0t* %3;
            }
        }
    }
}
/: