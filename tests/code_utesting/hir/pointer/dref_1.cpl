start() {
    ptr ptr ptr i32 a;
    ptr ptr i32 b = dref a;
    ptr i32 c = dref b;
    i32 d = dref c;
}

:/ OUTPUT
{
    start {
        {
            {
                i32s*** %0 = alloc;
                i32s** %1 = alloc;
                i32t** %4 = *(i32s*** %0);
                i32s** %1 = i32t** %4;
                i32s* %2 = alloc;
                i32t* %5 = *(i32s** %1);
                i32s* %2 = i32t* %5;
                i32s %3 = alloc;
                i32t %6 = *(i32s* %2);
                i32s %3 = i32t %6;
            }
        }
    }
}
/: