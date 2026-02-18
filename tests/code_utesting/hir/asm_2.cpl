{
    function foo() -> i32;
    start() {
        asm (10 + 10, foo()) {
            "mov rax, %0",
            "mov rbx, %1",
            "syscall"
        }
    }
}

: OUTPUT
{
    start {
        {
            {
                i64t %0 = num? 10 + num? 10;
                use i64t %0;
                i32t %1 = call foo0() -> i32, argc args();
                use i32t %1;
                asm, args(i64t %0,i32t %1,) {
                [raw, "mov rax, %0"]
                [raw, "mov rbx, %1"]
                [raw, "syscall"]
                }
            }
        }
    }
}
: