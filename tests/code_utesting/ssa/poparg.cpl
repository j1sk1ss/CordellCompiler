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
            i64s %0 = alloc(8);
            i64s %2 = load_starg();
            f32s* %1 = alloc(8);
            f32s* %3 = load_starg();
        }
    }
}
/:
