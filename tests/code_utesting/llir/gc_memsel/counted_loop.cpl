start() {
    @[counter(10)] loop {
    }
}

:/ OUTPUT
BB1: start
r15 = 10;
rdx = r15;
BB6: r15 = rdx;
rcx = r15;
BB2: lb12:
jmp lb13;
BB3: lb13:
r15 = rcx;
rax = r15;
rax = rax - 1;
r15 = rax;
rsi = r15;
r15 = rsi;
rbx = r15;
r15 = rbx;
rcx = r15;
cmp rbx, 0;
je lb14;
jne lb12;
BB4:
BB5: lb14:
send
/: