start() {
    exit 10 + 10;
}

: OUTPUT
=== BEFORE ===
BB{X} start
r15 = $10;
rax = r15;
rax = rax + $10;
r15 = rax;
rcx = r15;
r15 = rcx;
rdx = r15;
exit rdx;
BB{X} 
=== AFTER ===
BB{X} start
rax = $10;
rax = rax + $10;
rdx = rax;
exit rdx;
BB{X}
: