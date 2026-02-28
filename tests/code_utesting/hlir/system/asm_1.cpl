{
    start() {
        asm() {
            "mov rax, 1",
            "mov rbx, 1",
            "syscall"
        }
    }
}

: OUTPUT
BB{X}: start
[raw] (link: str(mov rax, 1)), arg[];
[raw] (link: str(mov rbx, 1)), arg[];
[raw] (link: str(syscall)), arg[];
send
: