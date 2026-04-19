: RUN_ASM :
start() {
    i32 a = 2;
    i32 b = ((i32 a) => a * a)(a) as i32 + '0';
    syscall(0x2000004, 1, ref b, 1);
}

:/ OUTPUT
4
/: