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
rcx = $1;
%19 = rcx;
[rbp - 32] = %19;
kill(12);
kill(13);
rcx = $1;
%20 = rcx;
[rbp - 16] = %20;
rcx = $1;
%21 = rcx;
[rbp - 32] = %21;
rcx = $1;
%22 = rcx;
[rbp - 48] = %22;
kill(15);
kill(16);
rcx = $1;
%23 = rcx;
[rbp - 32] = %23;
kill(14);
kill(17);
send
/: