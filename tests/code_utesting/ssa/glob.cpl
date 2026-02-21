{
    glob i32 a = 10;
    start() {
        a = 11;
        a = 12;
        a = 13;
    }
}

: OUTPUT
{
    i32g %0 = alloc(8);
    i32t %1 = num? 10 as i32;
    i32g %0 = i32t %1;
    start {
        {
            {
                i32t %2 = num? 11 as i32;
                i32g %5 = i32t %2;
                i32t %3 = num? 12 as i32;
                i32g %6 = i32t %3;
                i32t %4 = num? 13 as i32;
                i32g %7 = i32t %4;
            }
        }
    }
}
: