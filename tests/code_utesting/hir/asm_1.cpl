{
    start() {
        asm () {
            "mov rax, 1",
            "syscall"
        }
    }
}

: OUTPUT
{
    start {
        {
            {
                asm, args() {
                [raw, "mov rax, 1"]
                [raw, "syscall"]
                }
            }
        }
    }
}
: