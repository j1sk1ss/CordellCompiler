{
    function foo(ptr i8 msg);
    start() {
        str msg = "Hello world!";
        foo(msg);
    }
}

: OUTPUT
{
    start {
        {
            {
                strs %1 = str_alloc(Hello world!);
                use strs %1;
                call foo0(i8* msg), argc args(strs %1,);
            }
        }
    }
}
: