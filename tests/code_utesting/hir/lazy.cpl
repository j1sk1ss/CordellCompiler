start() {
    i32 a;
    i32 b;
    i8 c = (a && b) as i8;
    i8 d = (a || b) as i8;
}

:/ OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i32s %1 = alloc(8);
                i8s %2 = alloc(8);
                if i32s %0, goto lb21, else goto lb22;
                lb21:
                i32t %5 = i32s %1;
                goto lb23;
                lb22:
                i32t %5 = cnst? 0;
                goto lb23;
                lb23:
                i8t %4 = i32t %5 as i8;
                i8s %2 = i8t %4;
                i8s %3 = alloc(8);
                if i32s %0, goto lb38, else goto lb39;
                lb39:
                i32t %7 = i32s %1;
                goto lb40;
                lb38:
                i32t %7 = cnst? 1;
                goto lb40;
                lb40:
                i8t %6 = i32t %7 as i8;
                i8s %3 = i8t %6;
            }
        }
    }
}
/:
