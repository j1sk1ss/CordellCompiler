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
rcx = $1;
%18 = rcx;
[rbp - 16] = %18;
rbx = $1;
%19 = rbx;
[rbp - 32] = %19;
kill(12);
kill(13);
rax = $1;
%20 = rax;
[rbp - 16] = %20;
rdi = $1;
%21 = rdi;
[rbp - 32] = %21;
rdx = $1;
%22 = rdx;
[rbp - 48] = %22;
kill(15);
kill(16);
rsi = $1;
%23 = rsi;
[rbp - 32] = %23;
kill(14);
kill(17);
send
/: