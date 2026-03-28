    start() {
        i8 a;
        u8 b;
        i32 c = sizeof(a) as i32;
        i32 d = sizeof(b) as i32;
    }

: OUTPUT
{
    start {
        {
            {
                i8s %0 = alloc(8);
                u8s %1 = alloc(8);
                i32s %2 = alloc(8);
                i32t %4 = cnst? 1 as i32;
                i32s %2 = i32t %4;
                i32s %3 = alloc(8);
                i32t %5 = cnst? 1 as i32;
                i32s %3 = i32t %5;
            }
        }
    }
}
: