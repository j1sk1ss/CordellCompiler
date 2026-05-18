function foo() -> i32;
start() {
    syscall(0, foo(), 0 + 0, 0);
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i32t %0 = foo0();
            use i32t %0;
            i8t %1 = i8n 0 + i8n 0;
            use i8t %1;
            syscall(i8n 0, i32t %0, i8t %1, i8n 0);
        }
    }
}
/: