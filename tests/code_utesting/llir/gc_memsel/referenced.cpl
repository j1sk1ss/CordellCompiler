start() {
    i32 a = 0 as i32;
    ptr i32 b = ref a;
}

:/ OUTPUT
BB1: start
rcx = $0;
%6 = rcx;
[rbp - 8] = %6;
rcx = &([rbp - 8]);
rcx = rcx;
kill(4);
send
/: