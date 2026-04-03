start() {
    i32 a = 1 as i32;
    i32 b = 1 as i32;
    if a; exit b;
    else exit a;
}

:/ OUTPUT
BB1: start
rax = $1;
rsi = rax;
rdx = $1;
rcx = rdx;
cmp rsi, 0;
je lb29;
jne lb28;
BB2: lb28:
rdx = rcx;
exit rdx;
BB3: lb29:
rdx = rsi;
exit rdx;
BB4: lb30:
/: