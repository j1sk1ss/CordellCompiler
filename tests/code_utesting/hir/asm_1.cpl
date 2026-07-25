start() {
    asm () {
        "mov rax, 1",
        "syscall"
    }
}

:/ OUTPUT
{
    fn _main()
    {
        {
            asm() {
                [raw, "mov rax, 1"]
                [raw, "syscall"]
            }
        }
    }
}
/:
