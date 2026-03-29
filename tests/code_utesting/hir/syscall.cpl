function foo() -> i32;
start() {
    syscall(0, foo(), 0 + 0, 0);
}

:/ OUTPUT
{
    start {
        {
            {
                i32t %0 = call foo0() -> i32, argc args();
                use i32t %0;
                i8t %1 = i8n 0 + i8n 0;
                use i8t %1;
                syscall, argc: args(i8n 0,i32t %0,i8t %1,i8n 0,);
            }
        }
    }
}
/: