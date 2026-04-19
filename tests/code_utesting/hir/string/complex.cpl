start() {
    str msg = "Hello world!";
    ptr i8 msg1 = ref msg;
    msg1[0] = dref msg;
    msg[1] = msg1[0];
}

:/ OUTPUT
{
    start {
        {
            {
                strs %0 = str_alloc(Hello world!);
                i8s* %1 = alloc(8);
                i8t* %2 = &(strs %0);
                i8s* %1 = i8t* %2;
                i8t %3 = *(strs %0);
                i8t* %4 = i8s* %1;
                i64t %5 = i8n 0 * cnst? 1;
                i8t* %7 = i64t %5 as *ptr;
                i8t* %6 = i8t* %4 + i8t* %7;
                *(i8t* %6) = i8t %3;
                i8t* %8 = i8s* %1;
                i64t %9 = i8n 0 * cnst? 1;
                i8t* %11 = i64t %9 as *ptr;
                i8t* %10 = i8t* %8 + i8t* %11;
                i8t %12 = *(i8t* %10);
                i8t* %13 = &(strs %0);
                i64t %14 = i8n 1 * cnst? 1;
                i8t* %16 = i64t %14 as *ptr;
                i8t* %15 = i8t* %13 + i8t* %16;
                *(i8t* %15) = i8t %12;
            }
        }
    }
}
/: