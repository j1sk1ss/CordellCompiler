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
rax = $1;
rax <<= rax;
rax = $2;
rdi <<= rax;
rax = $3;
rsi <<= rax; : TODO
syscall;
[unused] kill(1);
[unused] kill(4);
[unused] kill(3);
[unused] kill(0);
[unused] kill(5);
[unused] kill(2);
send
: