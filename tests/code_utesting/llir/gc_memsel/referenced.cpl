start() {
    i32 a = 0 as i32;
    ptr i32 b = ref a;
}

:/ OUTPUT
BB1: start
rax = $0;
%6 = rax;
[rbp - 8] = %6;
rdx = &([rbp - 8]);
rcx = rdx;
kill(4);
send
/: