start() {
    @[counter(1000000000)] loop {
    }
    exit 0;
}

:/ OUTPUT
=== BEFORE ===
BB1: start
rdx = 1000000000;
BB7: kill(2);
rdx = rdx;
BB2: lb12:
jmp lb13;
BB3: lb13:
rax = rdx;
rax = rax - 1;
rdx = rax;
rbx = rdx;
rdx = rbx;
cmp rbx, 0;
je lb14;
jne lb12;
BB4:
BB5: lb14:
rdi = $0;
exit rdi;
BB6:
=== AFTER ===
BB1: start
rdx = 1000000000;
BB7: kill(2);
BB2: lb12:
jmp lb13;
BB3: lb13:
rdx--;
rax = rdx;
rbx = rax;
rdx = rax;
test rbx, rbx;
je lb14;
jne lb12;
BB4:
BB5: lb14:
rdi = rdi ^ rdi;
exit rdi;
BB6:
/: