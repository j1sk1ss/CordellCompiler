{
    start() {
        ptr i8 a;
        ptr ptr i8 b;
        dref a = 1;
        dref b = 1;
    }
}

: OUTPUT
{
    start {
        {
            {
                i8s* %0 = alloc(8);
                i8s** %1 = alloc(8);
                i8t %2 = num? 1 as i8;
                *(i8s* %0) = i8t %2;
                i8t* %3 = num? 1 as u64;
                *(i8s** %1) = i8t* %3;
            }
        }
    }
}
: