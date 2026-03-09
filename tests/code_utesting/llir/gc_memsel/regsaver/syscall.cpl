{
    start() {
        syscall(1, 2, 3);
    }
}

: OUTPUT
BB1: start
use $1;
use $2;
use $3;
r15 = $1;
rax <<= r15;
r15 = $2;
rdi <<= r15;
r15 = $3;
rsi <<= r15;
syscall;
send
: