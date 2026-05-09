function sum<U, T>(U a, T b) -> U {
    return a + b;
}

start() {
    ptr i0 p = sum<i8, i8>;
    ptr i0 p2 = sum<i16, i16>;
    ptr i0 p3 = sum<i8, u8>;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i0s* %2 = alloc;
            i0t* %11 = &(sum3__i16__i16(i16 a, i16 b) -> i16);
            i0s* %2 = i0t* %11;
            i0s* %3 = alloc;
            i0t* %12 = &(sum3__i16__i16(i16 a, i16 b) -> i16);
            i0s* %3 = i0t* %12;
            i0s* %4 = alloc;
            i0t* %13 = &(sum2__i8__u8(i8 a, u8 b) -> i8);
            i0s* %4 = i0t* %13;
        }
    }
    fn sum2__i8__u8(i8 a, u8 b) -> i8
    {
        i8s %6 = alloc;
        i8s %6 = load_arg();
        u8s %5 = alloc;
        u8s %5 = load_arg();
        {
            u8t %15 = i8s %6 as u8;
            u8t %14 = u8t %15 + u8s %5;
            i8t %16 = u8t %14 as i8;
            return i8t %16;
        }
    }
    fn sum3__i16__i16(i16 a, i16 b) -> i16
    {
        i16s %8 = alloc;
        i16s %8 = load_arg();
        i16s %7 = alloc;
        i16s %7 = load_arg();
        {
            i16t %17 = i16s %8 + i16s %7;
            return i16t %17;
        }
    }
    fn sum4__i8__i8(i8 a, i8 b) -> i8
    {
        i8s %10 = alloc;
        i8s %10 = load_arg();
        i8s %9 = alloc;
        i8s %9 = load_arg();
        {
            i8t %18 = i8s %10 + i8s %9;
            return i8t %18;
        }
    }
}
/: