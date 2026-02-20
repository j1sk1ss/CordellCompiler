{
    function foo(ptr ptr i32 a) {
        dref dref a;
    }

    start() {
        arr a[10, ptr i32];
        foo(ref a);
    }
}

: OUTPUT
{
    fn foo0(i32** a)
    {
        i32s** %0 = alloc(8);
        i32s** %0 = load_arg();
        {
        }
    }
    start {
        {
            {
                arrs %1 = arr_alloc(num? 10);
                i32t** %2 = &(arrs %1);
                use i32t** %2;
                call foo0(i32** a), argc args(i32t** %2,);
            }
        }
    }
}
: