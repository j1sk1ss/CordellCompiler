function foo(ptr i8 msg);
start() {
    str msg = "Hello world!";
    foo(msg);
}

:/ OUTPUT
{
    start {
        {
            {
                strs %1 = str_alloc(Hello world!);
                use strs %1;
                foo0(strs %1);
            }
        }
    }
}
/: