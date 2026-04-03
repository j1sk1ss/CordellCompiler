start() {
    syscall(1, 2, 3);
}

:/ OUTPUT
BB1: start
push(rax);
rax <<= $1;
push(rdi);
rdi <<= $2;
push(rsi);
rsi <<= $3;
syscall;
rsi = pop();
rdi = pop();
rax = pop();
send
/: