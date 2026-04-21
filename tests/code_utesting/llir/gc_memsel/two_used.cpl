start() {
    i32 a = 1 as i32;
    i32 b = 1 as i32;
    if a; exit b;
    else exit a;
}

:/ OUTPUT
BB1: start
rcx = $1;
rdx = rcx;
rcx = $1;
rcx = rcx;
cmp rdx, 0;
je lb18;
jne lb17;
BB2: lb17:
rdi = rcx;
exit rdi;
BB3: lb18:
rdi = rdx;
exit rdi;
BB4: lb19:
/: