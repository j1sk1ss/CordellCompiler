function foo(...) {
    @[poparg] i32 a;
    @[poparg] i32 b;
}

: OUTPUT
{
    fn foo0(...)
    {
        {
            i32s %0 = alloc(8);
            i32s %0 = load_arg();
            i32s %1 = alloc(8);
            i32s %1 = load_arg();
        }
    }
}
: