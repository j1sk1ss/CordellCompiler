{
    function foo(ptr i32 a);
    start() {
        arr a[10, i32];
        foo(a);
    }
}

: OUTPUT
{
    start {
        {
            {
                arrs %1 = arr_alloc(num? 10);
                use arrs %1;
                call foo0(i32* a), argc args(arrs %1,);
            }
        }
    }
}
: