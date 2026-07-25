function foo() -> i32;
start() {
    asm (10 + 10, foo()) {
        "mov rax, %0",
        "mov rbx, %1",
        "syscall"
    }
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i8t %0 = i8n 10 + i8n 10;
            use i8t %0;
            i32t %1 = foo0();
            use i32t %1;
            asm(i8t %0, i32t %1) {
                [raw, "mov rax, %0"]
                [raw, "mov rbx, %1"]
                [raw, "syscall"]
            }
        }
    }
}
/:
