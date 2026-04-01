start() {
    i32 a = 1;
    if 1; {
        a = 1;
    }
    i32 b = a;
}

:/ OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i32t %2 = i8n 1 as i32;
                i32s %4 = i32t %2;
                future: i32s %5 = previous: i32s %4;
                if i8n 1, goto lb18, else goto lb20;
                lb18:
                {
                    i32t %3 = i8n 1 as i32;
                    i32s %7 = i32t %3;
                }
                future: i32s %5 = previous: i32s %7;
                goto lb20;
                lb20:
                [i32s %0] i32s %5 = phi(set [%7, bb2][%4, bb1]);
                i32s %1 = alloc(8);
                i32s %6 = i32s %5;
            }
        }
    }
}
/: