{
    start() {
        i32 a;
        ptr i32 b = ref a;
        ptr ptr i32 c = ref b;
        i32 d = a;
        i32 f = c;
    }
}

: OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i32s* %1 = alloc(8);
                i32t* %5 = &(i32s %0);
                i32s* %1 = i32t* %5;
                i32s** %2 = alloc(8);
                i32t** %6 = &(i32s* %1);
                i32s** %2 = i32t** %6;
                i32s %3 = alloc(8);
                i32s %3 = i32s %0;
                i32s %4 = alloc(8);
                i32t %7 = i32s** %2 as i32;
                i32s %4 = i32t %7;
            }
        }
    }
}
: