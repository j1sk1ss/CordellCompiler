{
    start() {
        i32 a;
        i32 b = dref ref a;
    }
}

: OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i32s %1 = alloc(8);
                i32t* %2 = &(i32s %0);
                i32t %3 = *(i32t* %2);
                i32s %1 = i32t %3;
            }
        }
    }
}
: