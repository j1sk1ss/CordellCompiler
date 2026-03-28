start() {
    i32 a = 0 as i32;
    ptr i32 b = ref a;
}

:/ OUTPUT
BB1: start
r15 = $0;
rax = r15;
r15 = rax;
[rbp - 8] = r15;
r15 = &([rbp - 8]);
rdx = r15;
r15 = rdx;
rcx = r15;
kill(4);
send
/:
