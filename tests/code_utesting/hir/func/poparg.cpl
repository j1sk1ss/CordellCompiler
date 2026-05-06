@[entry]
function foo(...) {
    @[poparg] i64 a;
    @[poparg] ptr f32 b;
}

:/ OUTPUT
{
    fn _main(...)
    {
        {
            i64s %0 = alloc;
            i64s %0 = load_starg();
            f32s* %1 = alloc;
            f32s* %1 = load_starg();
        }
    }
}
/: