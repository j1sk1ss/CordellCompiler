start() {
    ptr i8 msg = "Hello, World!";
    arr ans[0, i8] = "Hello, User!";

    msg[0] = 'o' as i8;
    ans[1] = 'g' as i8;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i8s* %0 = alloc;
            i8s* %0 = Hello, World!;
            strs %1 = str_alloc(Hello, User!);
            i8t %2 = i8n 111 as i8;
            i8t* %3 = i8s* %0;
            i8t %4 = i8n 0 * csu8 1;
            i8t* %6 = i8t %4 as *ptr;
            i8t* %5 = i8t* %3 + i8t* %6;
            *(i8t* %5) = i8t %2;
            i8t %7 = i8n 103 as i8;
            i8t* %8 = &(strs %1);
            i8t %9 = i8n 1 * csu8 1;
            i8t* %11 = i8t %9 as *ptr;
            i8t* %10 = i8t* %8 + i8t* %11;
            *(i8t* %10) = i8t %7;
        }
    }
}
/: