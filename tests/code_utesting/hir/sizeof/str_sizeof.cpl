start() {
    str a = "Hello world1";
    str b = "world2";
    i32 c = sizeof(a);
    i32 d = sizeof(b);
}

: OUTPUT
{
    start {
        {
            {
                strs %0 = str_alloc(Hello world1);
                strs %1 = str_alloc(world2);
                i32s %2 = alloc(8);
                i32t %4 = cnst? 13 as i32;
                i32s %2 = i32t %4;
                i32s %3 = alloc(8);
                i32t %5 = cnst? 7 as i32;
                i32s %3 = i32t %5;
            }
        }
    }
}
: