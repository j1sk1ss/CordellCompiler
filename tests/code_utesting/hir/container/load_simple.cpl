@[align(1)]
container a {
    i32 a;
    i32 b;
}

start() {
    a nd;
    nd.a = 0;
    nd.b = 1;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            u8s %2 = arr_alloc(csu8 8);
            u8t* %3 = &(u8s %2);
            u8t %4 = u8t* %3 + csu8 0;
            *(u8t %4) = i8n 0;
            u8t* %5 = &(u8s %2);
            u8t %6 = u8t* %5 + csu8 4;
            *(u8t %6) = i8n 1;
        }
    }
}
/: