{
    function foo(u32 a);
    start() {
        ptr u32 a = foo;
        a(10);
        foo(10);
    }
}

: OUTPUT
{
    start {
        {
            {
                u32s* %1 = alloc(8);
                u64t* %2 = &(foo0(u32 a));
                u32t* %3 = u64t* %2 as *ptr;
                u32s* %1 = u32t* %3;
                use num? 10;
                call u32s* %1, argc args(num? 10,);
                use num? 10;
                call foo0(u32 a), argc args(num? 10,);
            }
        }
    }
}
: