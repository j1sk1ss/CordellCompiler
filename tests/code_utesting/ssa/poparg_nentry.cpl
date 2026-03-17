function foo(...) {
    @[poparg] i64 a;
    @[poparg] ptr f32 b;
}

start() {
    foo();
}

: OUTPUT
{
    fn foo0(...)
    {
        {
            i64s %0 = alloc(8);
            u64t %3 = load_arg();
            i64t %2 = u64t %3 as i64;
            i64s %6 = i64t %2;
            f32s* %1 = alloc(8);
            u64t %5 = load_arg();
            f32t* %4 = u64t %5 as *ptr;
            f32s* %7 = f32t* %4;
        }
    }
    start {
        {
            {
                call foo0(...), argc args();
            }
        }
    }
}
: