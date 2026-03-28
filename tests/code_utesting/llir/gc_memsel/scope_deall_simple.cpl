start() {
    {
        @[align(16)] i32 a = 1 as i32;
        @[align(16)] i32 b = 1 as i32;
    }

    @[align(16)] i32 a = 1 as i32;

    {
        @[align(16)] i32 b = 1 as i32;
        @[align(16)] i32 c = 1 as i32;
    }

    @[align(16)] i32 b = 1 as i32;
}

:/ OUTPUT
BB1: start
r15 = $1;
rbx = r15;
r15 = rbx;
[rbp - 16] = r15;
r15 = $1;
rcx = r15;
r15 = rcx;
[rbp - 32] = r15;
kill(12);
kill(13);
r15 = $1;
rdx = r15;
r15 = rdx;
[rbp - 16] = r15;
r15 = $1;
rdi = r15;
r15 = rdi;
[rbp - 32] = r15;
r15 = $1;
rax = r15;
r15 = rax;
[rbp - 48] = r15;
kill(15);
kill(16);
r15 = $1;
rsi = r15;
r15 = rsi;
[rbp - 32] = r15;
kill(14);
kill(17);
send
/:
