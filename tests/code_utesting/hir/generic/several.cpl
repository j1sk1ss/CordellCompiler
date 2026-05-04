function foo<U,T>(U a, T b) -> T {
    return a as T + b;
}

start() {
    exit foo<u8,i8>(1, 1);
}

:/ OUTPUT
{
    fn foo2__u8__i8(u8 a, i8 b) -> i8
    {
        u8s %3 = alloc;
        u8s %3 = load_arg();
        i8s %2 = alloc;
        i8s %2 = load_arg();
        {
            i8t %4 = u8s %3 as i8;
            i8t %5 = i8t %4 + i8s %2;
            return i8t %5;
        }
    }
    fn _main()
    {
        {
            i8t %6 = foo2__u8__i8(i8n 1, i8n 1);
            exit i8t %6;
        }
    }
}
/: