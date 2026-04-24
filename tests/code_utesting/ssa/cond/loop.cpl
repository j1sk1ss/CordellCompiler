start() {
    i32 a = 0;
    loop {
        a = 1;
        break;
    }
    i32 b = a;
}

:/ OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i32t %2 = i8n 0 as i32;
                i32s %4 = i32t %2;
                lb12:
                goto lb13;
                lb13:
                {
                    i32t %3 = i8n 1 as i32;
                    i32s %5 = i32t %3;
                    // break;
                    goto lb14;
                [unused] }
                [unused] goto lb12;
                lb14:
                i32s %1 = alloc(8);
                i32s %6 = i32s %5;
            }
        }
    }
}
/: