start() {
    @[counter(10)] loop {
    }
}

:/ OUTPUT
BB1: start
rdx = 10;
BB5: kill(2);
rdx = rdx;
BB2: lb9:
jmp lb10;
BB3: lb10:
rax = rdx;
rax = rax - 1;
rdx = rax;
rbx = rdx;
rdx = rbx;
cmp rbx, 0;
je lb11;
jne lb9;
BB4: lb11:
send
/: