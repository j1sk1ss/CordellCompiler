start() {
    @[counter(10)] loop {
    }
}

:/ OUTPUT
BB1: start
rdx = 10;
BB6: kill(2);
rbx = rdx;
BB2: lb12:
jmp lb13;
BB3: lb13:
rax = rbx;
rax = rax - 1;
rdi = rax;
rsi = rdi;
rbx = rsi;
cmp rsi, 0;
je lb14;
jne lb12;
BB4:
BB5: lb14:
send
/: