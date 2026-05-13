function sum<U, T>(U a, T b) -> U {
    return a + b;
}

start() {
    exit (sum<i8, i8>(1, 1) + sum<u8, u8>(2, 2) + sum<i16, i16>(3, 3) + sum<i32, i32>(4, 4)) as u8;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i8t %11 = sum2__i32__i32(i8n 1, i8n 1);
            u8t %12 = sum4__u8__u8(i8n 2, i8n 2);
            u8t %14 = i8t %11 as u8;
            u8t %13 = u8t %14 + u8t %12;
            i16t %15 = sum2__i32__i32(i8n 3, i8n 3);
            i16t %17 = u8t %13 as i16;
            i16t %16 = i16t %17 + i16t %15;
            i32t %18 = sum2__i32__i32(i8n 4, i8n 4);
            i32t %20 = i16t %16 as i32;
            i32t %19 = i32t %20 + i32t %18;
            u8t %10 = i32t %19 as u8;
            exit u8t %10;
        }
    }
    fn sum2__i32__i32(i32 a, i32 b) -> i32
    {
        i32s %3 = alloc;
        i32s %3 = load_arg();
        i32s %2 = alloc;
        i32s %2 = load_arg();
        {
            i32t %21 = i32s %3 + i32s %2;
            return i32t %21;
        }
    }
    fn sum3__i16__i16(i16 a, i16 b) -> i16
    {
        i16s %5 = alloc;
        i16s %5 = load_arg();
        i16s %4 = alloc;
        i16s %4 = load_arg();
        {
            i16t %22 = i16s %5 + i16s %4;
            return i16t %22;
        }
    }
    fn sum4__u8__u8(u8 a, u8 b) -> u8
    {
        u8s %7 = alloc;
        u8s %7 = load_arg();
        u8s %6 = alloc;
        u8s %6 = load_arg();
        {
            u8t %23 = u8s %7 + u8s %6;
            return u8t %23;
        }
    }
    fn sum5__i8__i8(i8 a, i8 b) -> i8
    {
        i8s %9 = alloc;
        i8s %9 = load_arg();
        i8s %8 = alloc;
        i8s %8 = load_arg();
        {
            i8t %24 = i8s %9 + i8s %8;
            return i8t %24;
        }
    }
}
/: