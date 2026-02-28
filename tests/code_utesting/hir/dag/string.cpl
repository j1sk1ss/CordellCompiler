{
    start() {
        str a = "Hello world!";
        str b = "Hello world!";
        ptr i8 c = "Hello world!";
        ptr i8 d = "Hello world!";
    }
}

: OUTPUT
{
    start {
        {
            {
                strs %0 = str_alloc(Hello world!);
                strs %1 = str_alloc(Hello world!);
                i8s* %2 = alloc(8);
                i8t* %4 = Hello world! as *ptr;
                i8s* %6 = i8t* %4;
                i8s* %3 = alloc(8);
                [unused] i8t* %5 = Hello world! as *ptr;
                i8s* %7 = i8t* %4;
            }
        }
    }
}
: