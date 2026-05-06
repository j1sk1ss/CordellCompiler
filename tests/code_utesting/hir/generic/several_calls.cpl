function sum<U,T>(U a, T b) -> T {
    return a as T + b;
}

start() {
    sum<i8,u8>(1, 1);
    sum<i8,u8>(1, 1);
}

:/ OUTPUT
{
    fn _main()
    {
        {
            sum2__i8__u8(i8n 1, i8n 1);
            sum2__i8__u8(i8n 1, i8n 1);
        }
    }
    fn sum2__i8__u8(i8 a, u8 b) -> u8
    {
        i8s %3 = alloc;
        i8s %3 = load_arg();
        u8s %2 = alloc;
        u8s %2 = load_arg();
        {
            u8t %4 = i8s %3 as u8;
            u8t %5 = u8t %4 + u8s %2;
            return u8t %5;
        }
    }
}
/: