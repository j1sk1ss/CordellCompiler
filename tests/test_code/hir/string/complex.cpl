{
    start() {
        str msg = "Hello world!";
        ptr str msg1 = ref msg;
        msg1[0] = dref msg;
        msg[1] = msg1[0];
    }
}

: OUTPUT
{
    start {
        {
            {
                strs* %0 = str_alloc(Hello world!);
                strs** %1 = alloc(8);
                i8t** %2 = &(strs* %0);
                i64t** %3 = i8t** %2 as u64;
                strs** %1 = i64t** %3;
                i8t %4 = *(strs* %0);
                u64t %5 = &(strs** %1);
                i64t %6 = num? 0 * cnst? 1;
                u64t %8 = i64t %6 as u64;
                u64t %7 = u64t %5 + u64t %8;
                i8t* %9 = i8t %4 as u64;
                *(u64t %7) = i8t* %9;
                u64t %10 = &(strs** %1);
                i64t %11 = num? 0 * cnst? 8;
                u64t %13 = i64t %11 as u64;
                u64t %12 = u64t %10 + u64t %13;
                i8t* %14 = *(u64t %12);
                u64t %15 = &(strs* %0);
                i64t %16 = num? 1 * cnst? 1;
                u64t %18 = i64t %16 as u64;
                u64t %17 = u64t %15 + u64t %18;
                i8t %19 = i8t* %14 as i8;
                *(u64t %17) = i8t %19;
            }
        }
    }
}
: