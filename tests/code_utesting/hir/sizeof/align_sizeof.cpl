start() {
    @[align(64)] i8 a;
    i32 b = sizeof(a) as i32;
}

: OUTPUT
{
    start {
        {
            {
                i8s %0 = alloc(8);
                i32s %1 = alloc(8);
                i32t %2 = cnst? 64 as i32;
                i32s %1 = i32t %2;
            }
        }
    }
}
: