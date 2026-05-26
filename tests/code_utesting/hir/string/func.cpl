function foo(ptr i8 msg) {
    msg[0] = 'A' as i8;
}

start() {
    foo(ref "Hello!");
}

:/ OUTPUT
{
    fn foo0(str msg)
    {
        strs %0 = alloc;
        strs %0 = load_arg();
        {
            i8t %1 = i8n 65 as i8;
            i8t* %2 = &(strs %0);
            i8t %3 = i8n 0 * csu8 1;
            i8t* %5 = i8t %3 as *ptr;
            i8t* %4 = i8t* %2 + i8t* %5;
            *(i8t* %4) = i8t %1;
        }
    }
    fn _main()
    {
        {
            use Hello!;
            foo0(Hello!);
        }
    }
}
/: