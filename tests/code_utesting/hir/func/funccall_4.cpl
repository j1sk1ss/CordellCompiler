{
    function foo(u32 a);
    start() {
        ptr i0 a = foo;
        a(10);
        foo(10);
    }
}

: OUTPUT
{
    start {
        {
            {
                i0s* %1 = alloc(8);
                i0t* %2 = &(foo0(u32 a));
                i0s* %1 = i0t* %2;
                use num? 10;
                call i0s* %1, argc args(num? 10,);
                use num? 10;
                call foo0(u32 a), argc args(num? 10,);
            }
        }
    }
}
: