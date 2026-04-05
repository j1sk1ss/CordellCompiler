start() {
    exit 10 + 10;
}

:/ OUTPUT
=== BEFORE ===
BB1: start
rax = $10;
rax = rax + $10;
rdx = rax;
rdx = rdx;
exit rdx;
BB2:
=== AFTER ===
BB1: start
rax = $10;
rax = rax + $10;
rdx = rax;
exit rdx;
BB2:
/: