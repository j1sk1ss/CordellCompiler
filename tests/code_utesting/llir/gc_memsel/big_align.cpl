{
    start() {
        @[align(16)] i32 a = 1;
    }
}

: OUTPUT
BB1: start
rax = $1;
rdx = rax;
rcx = rdx;
[rbp - 16] = rcx;
[unused] kill(1);
[unused] kill(4);
[unused] kill(3);
kill(2);
send
: