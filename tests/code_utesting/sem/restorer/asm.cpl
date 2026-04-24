function foo(i32 a, ptr i8 b) -> i0 {
    asm (a, b) {
        "mov rax, %0",
        "mov rbx, %1",
        "syscall"
    }
}

start() {
    foo(10, ref "Hello world!");
}

:/ OUTPUT
{X}2 | {
 1 |     function foo(i32 a, ptr i8 b) -> i0
 2 |     {
 2 |         asm(a, b)        {
 3 |             "mov rax, %0";
 4 |             "mov rbx, %1";
 5 |             "syscall";
 2 |         }
 2 |     }
 9 |     start ()
10 |     {
10 |         foo(10, ref "Hello world!");
10 |     }
 2 | }
/: