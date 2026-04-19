: RUN_ASM :
function print(ptr i8 msg, i32 len) -> i0 {
    syscall(0x2000004, 1, msg, len);
}

start() {
    print(ref "Hello, world!\n", 14 as i32);
}

:/ OUTPUT
Hello, world!
/: