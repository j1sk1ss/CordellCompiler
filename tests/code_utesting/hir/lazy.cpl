{
    start() {
        i32 a;
        i32 b;
        i8 c = a && b;
        i8 d = a || b;
    }
}

: OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i32s %1 = alloc(8);
                i8s %2 = alloc(8);
                if i32s %0, goto lb8, else goto lb9;
                lb8:
                i32t %4 = i32s %1;
                goto lb10;
                lb9:
                i32t %4 = cnst? 0;
                goto lb10;
                lb10:
                i8t %5 = i32t %4 as i8;
                i8s %2 = i8t %5;
                i8s %3 = alloc(8);
                if i32s %0, goto lb18, else goto lb19;
                lb19:
                i32t %6 = i32s %1;
                goto lb20;
                lb18:
                i32t %6 = cnst? 1;
                goto lb20;
                lb20:
                i8t %7 = i32t %6 as i8;
                i8s %3 = i8t %7;
            }
        }
    }
}
: