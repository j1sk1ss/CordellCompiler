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
rdx = $1;
%18 = rdx;
[rbp - 16] = %18;
rsi = $1;
%19 = rsi;
[rbp - 32] = %19;
kill(12);
kill(13);
rcx = $1;
%20 = rcx;
[rbp - 16] = %20;
r8 = $1;
%21 = r8;
[rbp - 32] = %21;
rbx = $1;
%22 = rbx;
[rbp - 48] = %22;
kill(15);
kill(16);
rdi = $1;
%23 = rdi;
[rbp - 32] = %23;
kill(14);
kill(17);
send
/: