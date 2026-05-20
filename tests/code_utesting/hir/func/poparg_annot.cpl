function foo(...) {
    @[poparg] i32 a;
    @[poparg] i32 b;
}

:/ OUTPUT
{
    fn foo0(...)
    {
        i0s* %2 = &(*)
        {
            i32s %0 = alloc;
            i32s %0 = *(i0s* %2);
            i0t* %3 = i0s* %2 + csu8 4;
            i0s* %2 = i0t* %3;
            i32s %1 = alloc;
            i32s %1 = *(i0s* %2);
            i0t* %4 = i0s* %2 + csu8 4;
            i0s* %2 = i0t* %4;
        }
    }
}
/: