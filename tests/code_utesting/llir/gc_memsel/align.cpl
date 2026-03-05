{
    start() {
        @[align(8)] i32 a = 1;
        @[align(64)] i32 b = 1;
    }
}

: OUTPUT
BB1: start
rbx = $1;
rax = rbx;
rcx = rax;
rsi = rcx;
rbp = $1;
rdx = rbp;
rdi = rdx;
[rbp - 64] = rdi;
[unused] kill(8);
[unused] kill(9);
[unused] kill(4);
[unused] kill(7);
[unused] kill(3);
kill(5);
[unused] kill(6);
[unused] kill(2);
send
: