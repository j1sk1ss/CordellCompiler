start() {
    i32 a = 10;
    i32 b = 11;

    while 1; {
        a = 100;
        b = 100;
    }
}

:/ OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i32t %2 = i8n 10 as i32;
                i32s %6 = i32t %2;
                i32s %1 = alloc(8);
                i32t %3 = i8n 11 as i32;
                i32s %7 = i32t %3;
                future: i32s %8 = previous: i32s %6;
                future: i32s %9 = previous: i32s %7;
                i32t %4 = i8n 100 as i32;
                i32s %10 = i32t %4;
                i32t %5 = i8n 100 as i32;
                i32s %11 = i32t %5;
                lb17:
                [i32s %0] i32s %8 = phi(set [%10, bb3][%6, bb5]);
                [i32s %1] i32s %9 = phi(set [%7, bb5][%11, bb3]);
                if i8n 1, goto lb18, else goto lb19;
                lb18:
                {
                }
                future: i32s %8 = previous: i32s %10;
                future: i32s %9 = previous: i32s %11;
                goto lb17;
                lb19:
            }
        }
    }
}
/: