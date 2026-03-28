start() {
    ptr i8 msg = "Hello, World!";
    str ans = "Hello, User!";

    msg[0] = 'o' as i8;
    ans[1] = 'g' as i8;
}

: OUTPUT
{
    start {
        {
            {
                i8s* %0 = alloc(8);
                i8t* %2 = Hello, World! as *ptr;
                i8s* %0 = i8t* %2;
                strs %1 = str_alloc(Hello, User!);
                i8t %3 = num? 111 as i8;
                i8t* %4 = i8s* %0;
                i64t %5 = num? 0 * cnst? 1;
                i8t* %7 = i64t %5 as *ptr;
                i8t* %6 = i8t* %4 + i8t* %7;
                *(i8t* %6) = i8t %3;
                i8t %8 = num? 103 as i8;
                i8t* %9 = &(strs %1);
                i64t %10 = num? 1 * cnst? 1;
                i8t* %12 = i64t %10 as *ptr;
                i8t* %11 = i8t* %9 + i8t* %12;
                *(i8t* %11) = i8t %8;
            }
        }
    }
}
: