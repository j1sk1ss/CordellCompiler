start() {
    ptr i8 a;
    ptr u64 b;
    i32 c = sizeof(a);
    i32 d = sizeof(b);
}

: OUTPUT
{
    start {
        {
            {
                i8s* %0 = alloc(8);
                u64s* %1 = alloc(8);
                i32s %2 = alloc(8);
                i32t %4 = cnst? 8 as i32;
                i32s %2 = i32t %4;
                i32s %3 = alloc(8);
                i32t %5 = cnst? 8 as i32;
                i32s %3 = i32t %5;
            }
        }
    }
}
: