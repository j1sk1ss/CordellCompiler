: BUG :
container storage {
    function init<T>(ptr storage self) -> T {
    }

    @[static]
    function sum<T, U>(T a, U b) -> U {
        return a as U + b;
    }
}

start() {
    storage s;
    s.init<i32>();
    s.sum<i8, u8>(1, 2);
}

:/ OUTPUT
{
    fn _main()
    {
        {
            u8s %3 = arr_alloc(csu8 0);
            u8t* %6 = &(u8s %3);
            use u8t* %6;
            init4__i32(u8t* %6);
            sum3__i8__u8(i8n 1, i8n 2);
        }
    }
    fn sum3__i8__u8( a,  b) ->
    {
        i8s %5 = alloc;
        i8s %5 = load_arg();
        u8s %4 = alloc;
        u8s %4 = load_arg();
        {
            u8t %7 = i8s %5 as u8;
            u8t %8 = u8t %7 + u8s %4;
            i8t %9 = u8t %8 as i8;
            return i8t %9;
        }
    }
    fn init4__i32(u8* self) ->
    {
        u8s* %0 = alloc;
        u8s* %0 = load_arg();
        {
        }
    }
}
/: