start() {
    i16 a;
    u16 b;
    i32 c = sizeof(a);
    i32 d = sizeof(b);
}

: OUTPUT
{
    start {
        {
            {
                i16s %0 = alloc(8);
                u16s %1 = alloc(8);
                i32s %2 = alloc(8);
                i32t %4 = cnst? 2 as i32;
                i32s %2 = i32t %4;
                i32s %3 = alloc(8);
                i32t %5 = cnst? 2 as i32;
                i32s %3 = i32t %5;
            }
        }
    }
}
: