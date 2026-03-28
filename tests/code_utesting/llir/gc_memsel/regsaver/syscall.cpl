start() {
    syscall(1, 2, 3);
}

:/ OUTPUT
BB{X} start
r15 = $1;
rax <<= r15;
r15 = $2;
rdi <<= r15;
r15 = $3;
rsi <<= r15;
syscall;
send
/:
