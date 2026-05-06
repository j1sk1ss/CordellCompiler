start() {
    @[counter(1000000000)] loop {
    }
    exit 0;
}

:/ OUTPUT
=== BEFORE ===
BB1: start
rcx = 1000000000;
BB6: kill(2);
rcx <<= rcx;
BB2: lb9:
jmp lb10;
BB3: lb10:
rax = rcx;
rax = rax - 1;
rcx = rax;
rdx = rcx;
rcx <<= rdx;
cmp rdx, 0;
je lb11;
jne lb9;
BB4: lb11:
rdi = $0;
exit rdi;
BB5: send
=== AFTER ===
BB1: start
rcx = 1000000000;
BB6: kill(2);
BB2:
BB3: lb10:
rcx = rcx - 1;
jne lb10;
BB4: rdi = rdi ^ rdi;
exit rdi;
BB5: send
/: