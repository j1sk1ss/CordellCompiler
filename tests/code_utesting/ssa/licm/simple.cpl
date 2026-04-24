start() {
    i32 d = 0;
    while 1; {
        i32 c = 10 + 10;
        d += c;
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
                [i32s %0] i32s %7 = phi(set [%9, bb3][%6, bb6]);
                [i32s %1] i32s %10 = phi(set [%8, bb3]);
                if i8n 1, goto lb13, else goto lb14;
                lb13:
                {
                    i32s %1 = alloc(8);
                    i32s %8 = i32t %4;
                    i32t %5 = i32s %7 + i32s %8;
                    i32s %9 = i32t %5;
                }
                future: i32s %7 = previous: i32s %9;
                future: i32s %10 = previous: i32s %8;
                goto lb12;
                lb14:
                exit i8n 1;
            }
        }
    [unused] }
}
/: