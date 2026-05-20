@[entry]
function foo(...) {
    @[poparg] i64 a;
    @[poparg] ptr f32 b;
}

:/ OUTPUT
{
    fn _main(...)
    {
        i0s* %2 = &(*)
        {
            i64s %0 = alloc;
            i64s %0 = *(i0s* %2);
            i0t* %3 = i0s* %2 + csu8 8;
            i0s* %2 = i0t* %3;
            f32s* %1 = alloc;
            f32s* %1 = *(i0s* %2);
            i0t* %4 = i0s* %2 + csu8 8;
            i0s* %2 = i0t* %4;
        }
    }
}
/: