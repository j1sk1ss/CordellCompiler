{
    glob i32 a;
    start() {
        i32 b = a + 1;
        a = b;
    }
}

: OUTPUT
{
    i32g %0 = alloc(8);
    start {
        {
            {
                i32s %1 = alloc(8);
                i64t %3 = i32g %0 as i64;
                i64t %2 = i64t %3 + num? 1;
                i32t %4 = i64t %2 as i32;
                i32s %1 = i32t %4;
                i32g %0 = i32s %1;
            }
        }
    }
}
: