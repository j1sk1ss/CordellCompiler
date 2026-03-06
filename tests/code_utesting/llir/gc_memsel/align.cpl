{
    start() {
        @[align(8)] i32 a = 1;
        @[align(64)] i32 b = 1;
    }
}

: OUTPUT
BB1: start
r15 = $1;
rax = r15;
r15 = rax;
rdx = r15;
r15 = $1;
rcx = r15;
r15 = rcx;
[rbp - 64] = r15;
kill(5);
send
: