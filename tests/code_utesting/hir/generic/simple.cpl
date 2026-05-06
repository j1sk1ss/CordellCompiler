function foo<T>(T a, T b) -> T {
    T d = 10 as T;
    i32 e = 10;
    function local() -> i32 {
        return 10;
    }
    return d + local() as T;
}

start() {
    exit foo<u8>(10, 10) + foo<i8>(10, 10);
}

:/ OUTPUT
{
    fn _main()
    {
        {
            u8t %12 = foo4__u8(i8n 10, i8n 10);
            i8t %13 = foo3__i8(i8n 10, i8n 10);
            u8t %15 = i8t %13 as u8;
            u8t %14 = u8t %12 + u8t %15;
            exit u8t %14;
        }
    }
    fn foo3__i8(i8 a, i8 b) -> i8
    {
        i8s %7 = alloc;
        i8s %7 = load_arg();
        i8s %6 = alloc;
        i8s %6 = load_arg();
        {
            i8s %5 = alloc;
            i8t %16 = i8n 10 as i8;
            i8s %5 = i8t %16;
            i32s %4 = alloc;
            i32t %17 = i8n 10 as i32;
            i32s %4 = i32t %17;
            goto lb43;
            fn local5() -> i32
            {
                {
                    i32t %18 = i8n 10 as i32;
                    return i32t %18;
                }
            }
            lb43:
            i32t %20 = local5();
            i8t %19 = i32t %20 as i8;
            i8t %21 = i8s %5 + i8t %19;
            return i8t %21;
        }
    }
    fn foo4__u8(u8 a, u8 b) -> u8
    {
        u8s %11 = alloc;
        u8s %11 = load_arg();
        u8s %10 = alloc;
        u8s %10 = load_arg();
        {
            u8s %9 = alloc;
            u8t %22 = i8n 10 as u8;
            u8s %9 = u8t %22;
            i32s %8 = alloc;
            i32t %23 = i8n 10 as i32;
            i32s %8 = i32t %23;
            goto lb82;
            fn local6() -> i32
            {
                {
                    i32t %24 = i8n 10 as i32;
                    return i32t %24;
                }
            }
            lb82:
            i32t %26 = local6();
            u8t %25 = i32t %26 as u8;
            u8t %27 = u8s %9 + u8t %25;
            return u8t %27;
        }
    }
}
/: