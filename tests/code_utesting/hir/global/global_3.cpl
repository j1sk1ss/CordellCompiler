{
    glob i32 a;
    glob ptr ptr u64 b;
    start() {
        a = 10 as i32;
        b = 0 as ptr ptr u64;
        i32 a;
        ptr ptr u64 b;
    }
}

: OUTPUT
{
    i32g %0 = alloc(8);
    u64g** %1 = alloc(8);
    start {
        {
            {
                i32t %4 = num? 10 as i32;
                i32g %0 = i32t %4;
                u64t** %5 = num? 0 as *ptr;
                u64g** %1 = u64t** %5;
                i32s %2 = alloc(8);
                u64s** %3 = alloc(8);
            }
        }
    }
}
: