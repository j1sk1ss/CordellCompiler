function foo<T>(T a, T b) -> T {
    T d = 10 as T;
    i32 e = 10;
    function local() -> i32 {
        return 10;
    }
    return d + local();
}

start() {
    exit foo<u8>(10, 10) + foo<i8>(10, 10);
}

:/ OUTPUT
{
    fn foo3__i8(i8 a, i8 b) -> i8
    {
        i8s %7 = alloc;
        i8s %7 = load_arg();
        i8s %6 = alloc;
        i8s %6 = load_arg();
        {
            i8s %5 = alloc;
            i8t %8 = i8n 10 as i8;
            i8s %5 = i8t %8;
            i32s %4 = alloc;
            i32t %9 = i8n 10 as i32;
            i32s %4 = i32t %9;
            goto lb23;
            fn local5() -> i32
            {
                {
                    return i8n 10;
                }
            }
            lb23:
            i32t %10 = local5();
            i32t %12 = i8s %5 as i32;
            i32t %11 = i32t %12 + i32t %10;
            return i32t %11;
        }
    }
    fn foo4__u8(u8 a, u8 b) -> u8
    {
        u8s %16 = alloc;
        u8s %16 = load_arg();
        u8s %15 = alloc;
        u8s %15 = load_arg();
        {
            u8s %14 = alloc;
            u8t %17 = i8n 10 as u8;
            u8s %14 = u8t %17;
            i32s %13 = alloc;
            i32t %18 = i8n 10 as i32;
            i32s %13 = i32t %18;
            goto lb61;
            fn local6() -> i32
            {
                {
                    return i8n 10;
                }
            }
            lb61:
            i32t %19 = local6();
            i32t %21 = u8s %14 as i32;
            i32t %20 = i32t %21 + i32t %19;
            return i32t %20;
        }
    }
    fn _main()
    {
        {
            u8t %22 = foo4__u8(i8n 10, i8n 10);
            i8t %23 = foo3__i8(i8n 10, i8n 10);
            u8t %25 = i8t %23 as u8;
            u8t %24 = u8t %22 + u8t %25;
            exit u8t %24;
        }
    }
}
/: