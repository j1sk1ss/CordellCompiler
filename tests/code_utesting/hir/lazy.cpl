start() {
    i32 a;
    i32 b;
    i8 c = (a && b) as i8;
    i8 d = (a || b) as i8;
}

: OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i32s %1 = alloc(8);
                i8s %2 = alloc(8);
                if i32s %0, goto lb24, else goto lb25;
                lb24:
                i32t %5 = i32s %1;
                goto lb26;
                lb25:
                i32t %5 = cnst? 0;
                goto lb26;
                lb26:
                i8t %4 = i32t %5 as i8;
                i8s %2 = i8t %4;
                i8s %3 = alloc(8);
                if i32s %0, goto lb45, else goto lb46;
                lb46:
                i32t %7 = i32s %1;
                goto lb47;
                lb45:
                i32t %7 = cnst? 1;
                goto lb47;
                lb47:
                i8t %6 = i32t %7 as i8;
                i8s %3 = i8t %6;
            }
        }
    }
}
: