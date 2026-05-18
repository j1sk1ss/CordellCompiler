function foo(ptr i8 msg);
start() {
    str msg = "Hello world!";
    foo(msg);
}

:/ OUTPUT
{
    fn _main()
    {
        {
            strs %1 = str_alloc(Hello world!);
            use strs %1;
            foo0(strs %1);
        }
    }
}
/: