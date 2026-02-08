{
    start() {
        ptr i32 a;
        ptr ptr i32 b = ref a;
    }
}

: OUTPUT
{
    start {
        {
            {
                i32s* %0 = alloc(8);
                i32s** %1 = alloc(8);
                i32t** %2 = &(i32s* %0);
                i32s** %1 = i32t** %2;
            }
        }
    }
}
: