function foo(...) {
    @[poparg] i64 a;
    @[poparg] ptr f32 b;
}

start() {
    foo();
}

:/ OUTPUT
{
    fn foo0(...)
    {
        {
            i64s %0 = alloc;
            i64s %2 = load_arg();
            f32s* %1 = alloc;
            f32s* %3 = load_arg();
        }
    }
    fn _main()
    {
        {
            foo0();
        }
    }
}
/: