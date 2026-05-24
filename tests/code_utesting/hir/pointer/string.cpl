start() {
    arr msg[10, i8] = "Hello world!\0";
    msg[0] = '0';
}

:/ OUTPUT
{
    fn _main()
    {
        {
            arrs %0 = str_alloc(Hello world!);
            i8t* %1 = &(arrs %0);
            i8t %2 = i8n 0 * csu8 1;
            i8t* %4 = i8t %2 as *ptr;
            i8t* %3 = i8t* %1 + i8t* %4;
            *(i8t* %3) = i8n 48;
        }
    }
}
/: