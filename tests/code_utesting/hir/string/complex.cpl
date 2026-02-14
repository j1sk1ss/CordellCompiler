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
                strs %0 = str_alloc(Hello world!);
                strs* %1 = alloc(8);
                i8t* %2 = &(strs %0);
                i64t* %3 = i8t* %2 as u64;
                strs* %1 = i64t* %3;
                i8t %4 = *(strs %0);
                i8t** %5 = &(strs* %1);
                i64t %6 = num? 0 * cnst? 1;
                i8t** %8 = i64t %6 as u64;
                i8t** %7 = i8t** %5 + i8t** %8;
                *(i8t** %7) = i8t %4;
                i8t** %9 = &(strs* %1);
                i64t %10 = num? 0 * cnst? 1;
                i8t** %12 = i64t %10 as u64;
                i8t** %11 = i8t** %9 + i8t** %12;
                i8t* %13 = *(i8t** %11);
                i8t* %14 = &(strs %0);
                i64t %15 = num? 1 * cnst? 1;
                i8t* %17 = i64t %15 as u64;
                i8t* %16 = i8t* %14 + i8t* %17;
                i8t %18 = i8t* %13 as i8;
                *(i8t* %16) = i8t %18;
            }
        }
    }
}
: