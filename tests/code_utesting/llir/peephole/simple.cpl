start() {
    exit 10 + 10;
}

:/ OUTPUT
=== BEFORE ===
BB1: start
rax = $10;
rax = rax + $10;
rcx = rax;
rdx = rcx;
exit rdx;
BB2:
=== AFTER ===
BB1: start
rax = $10;
rax = rax + $10;
rcx = rax;
rdx = rax;
exit rdx;
BB2:
/: