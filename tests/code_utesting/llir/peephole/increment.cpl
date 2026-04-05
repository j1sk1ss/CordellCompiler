start() {
    i32 a = 1 as i32;
    a += 1 as i32;
    exit a;
}

:/ OUTPUT
=== BEFORE ===
BB1: start
r9 = $1;
r8 = r9;
rdx = $1;
rax = r8;
rax = rax + rdx;
rdi = rax;
rbx = rdi;
rdx = rbx;
exit rdx;
BB2:
=== AFTER ===
BB1: start
r9 = $1;
r8 = $1;
rdx = $1;
rax = $1;
rax = rax + rdx;
rdi = rax;
rbx = rax;
rdx = rax;
exit rdx;
BB2:
/: