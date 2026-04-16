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
                future: i32s %8 = previous: i32s %1;
                future: i32s %7 = previous: i32s %6;
                i8t %3 = i8n 10 + i8n 10;
                i32t %4 = i8t %3 as i32;
                lb18:
                [i32s %0] i32s %7 = phi(set [%6, bb9][%12, bb6]);
                [i32s %1] i32s %8 = phi(set [%11, bb6][%1, bb9]);
                future: i32s %9 = previous: i32s %7;
                future: i32s %10 = previous: i32s %8;
                if i8n 1, goto lb19, else goto lb20;
                lb19:
                {
                    i32s %8 = alloc(8);
                    i32s %11 = i32t %4;
                    i32t %5 = i32s %7 + i32s %11;
                    i32s %12 = i32t %5;
                    if i32s %12, goto lb46, else goto lb48;
                    lb46:
                    {
                        // break;
                        future: i32s %9 = previous: i32s %12;
                        future: i32s %10 = previous: i32s %11;
                        goto lb20;
                    [unused] }
                    [unused] goto lb48;
                    lb48:
                }
                future: i32s %7 = previous: i32s %12;
                future: i32s %8 = previous: i32s %11;
                goto lb18;
                lb20:
                [i32s %0] i32s %9 = phi(set [%7, bb2][%12, bb4]);
                [i32s %1] i32s %10 = phi(set [%8, bb2][%11, bb4]);
                exit i8n 1;
            }
        }
    [unused] }
}
/: