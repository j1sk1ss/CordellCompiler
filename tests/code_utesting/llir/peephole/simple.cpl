start() {
    exit 10 + 10;
}

:/ OUTPUT
=== BEFORE ===
BB1: start
rax = $10;
rax = rax + $10;
rcx = rax;
rdi = rcx;
exit rdi;
BB2: send
=== AFTER ===
BB1: start
rax = $10;
rax = rax + $10;
rcx = rax;
rdi = rcx;
exit rdi;
BB2: send
/: