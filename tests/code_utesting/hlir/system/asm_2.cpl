{
    start() {
        i32 a;
        i32 b;
        asm(a, b) {
            "mov rax, %0",
            "mov rbx, %1"
        }
    }
}

: OUTPUT
BB1: start
use %0;
use %1;
[raw] (link: str(mov rax, %0)), arg[%0];
[raw] (link: str(mov rbx, %1)), arg[%1];
send
: