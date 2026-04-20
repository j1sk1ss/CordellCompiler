start() {
    @[counter(1000000000)] loop {
    }
    exit 0;
}

:/ OUTPUT
=== BEFORE ===
BB1: start
rdx = 1000000000;
BB6: kill(2);
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
rdi = $0;
exit rdi;
BB5:
=== AFTER ===
BB1: start
rdx = 1000000000;
BB6: kill(2);
BB2: lb9:
jmp lb10;
BB3: lb10:
rax = rdx;
rax = rax - 1;
rbx = rax;
rdx = rax;
test rbx, rbx;
je lb11;
jne lb9;
BB4: lb11:
rdi = rdi ^ rdi;
exit rdi;
BB5:
/: