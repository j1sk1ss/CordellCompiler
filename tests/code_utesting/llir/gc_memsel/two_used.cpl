start() {
    i32 a = 1 as i32;
    i32 b = 1 as i32;
    if a; exit b;
    else exit a;
}

:/ OUTPUT
BB1: start
rcx = $1;
rdi = rcx;
rbx = $1;
rdx = rbx;
cmp rdi, 0;
je lb29;
jne lb28;
BB2: lb28:
rdx = rdx;
exit rdx;
BB3: lb29:
rdx = rdi;
exit rdx;
BB4: lb30:
/: