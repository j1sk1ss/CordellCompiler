{
    function foo(i32 a) {
        foo(a + 1);
    }
}

: OUTPUT
{
    fn foo0(i32 a)
    {
        i32s %0 = alloc(8);
        i32s %0 = load_arg();
        {
            lb17:
            i64t %2 = i32s %0 as i64;
            i64t %1 = i64t %2 + num? 1;
            use i64t %1;
            i32s %0 = i64t %1;
            goto lb17;
            [unused] excall , argc foo0(i32 a)args(i64t %1,);
        }
    }
}
: