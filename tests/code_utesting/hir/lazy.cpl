start() {
    i32 a;
    i32 b;
    i8 c = (a && b) as i8;
    i8 d = (a || b) as i8;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i32s %0 = alloc;
            i32s %1 = alloc;
            i8s %2 = alloc;
            i64s %5 = alloc;
            if i32s %0, goto lb15, else goto lb16;
            lb15:
            i64s %5 = alloc;
            i64t %6 = i32s %1 as i64;
            i64s %5 = i64t %6;
            goto lb17;
            lb16:
            i64s %5 = csu8 0;
            goto lb17;
            lb17:
            i8t %4 = i64s %5 as i8;
            i8s %2 = i8t %4;
            i8s %3 = alloc;
            i64s %8 = alloc;
            if i32s %0, goto lb29, else goto lb30;
            lb30:
            i64t %9 = i32s %1 as i64;
            i64s %8 = i64t %9;
            goto lb31;
            lb29:
            i64s %8 = csu8 1;
            goto lb31;
            lb31:
            i8t %7 = i64s %8 as i8;
            i8s %3 = i8t %7;
        }
    }
}
/: