start() {
    @[counter(10)] loop {
    }
}

:/ OUTPUT
BB1: start
rdx <<= rcx;
rcx = 10;
BB2: lb9:
jmp lb10;
BB3: lb10:
rax = rdx;
rax = rax - 1;
rcx = rax;
rcx = rcx;
rdx <<= rcx;
cmp rcx, 0;
je lb11;
jne lb9;
BB4: lb11:
send
/: