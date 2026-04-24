start() {
    i32 d = 0;
    while 1; {
        i32 c = 10 + 10;
        d += c;
        if d; {
            break;
        }
    }

    exit 1;
}

:/ OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i32t %2 = i8n 0 as i32;
                i32s %6 = i32t %2;
                future: i32s %7 = previous: i32s %6;
                i8t %3 = i8n 10 + i8n 10;
                i32t %4 = i8t %3 as i32;
                lb12:
                [i32s %0] i32s %7 = phi(set [%6, bb8][%10, bb5]);
                [i32s %1] i32s %11 = phi(set [%9, bb5]);
                future: i32s %8 = previous: i32s %7;
                if i8n 1, goto lb13, else goto lb14;
                lb13:
                {
                    i32s %1 = alloc(8);
                    i32s %9 = i32t %4;
                    i32t %5 = i32s %7 + i32s %9;
                    i32s %10 = i32t %5;
                    if i32s %10, goto lb30, else goto lb32;
                    lb30:
                    {
                        // break;
                        future: i32s %8 = previous: i32s %10;
                        future: i32s %12 = previous: i32s %9;
                        goto lb14;
                    [unused] }
                    [unused] goto lb32;
                    lb32:
                }
                future: i32s %7 = previous: i32s %10;
                future: i32s %11 = previous: i32s %9;
                goto lb12;
                lb14:
                [i32s %0] i32s %8 = phi(set [%7, bb2][%10, bb4]);
                [i32s %1] i32s %12 = phi(set [%9, bb4]);
                exit i8n 1;
            }
        }
    [unused] }
}
/: