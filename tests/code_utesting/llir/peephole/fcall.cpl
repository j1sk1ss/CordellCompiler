function foo(i32 a) -> i32 {
    return a + 100 as i32;
}

start() {
    exit foo(100 as i32);
}

:/ OUTPUT
=== BEFORE ===
BB2: fn foo0(i32 a) -> i32
r15 = rdi;
rbx = r15;
r15 = $100;
rdx = r15;
r15 = rbx;
rax = r15;
rax = rax + rdx;
r15 = rax;
rcx = r15;
r15 = rcx;
rax = r15;
return rax;
BB3:
BB4: start
r15 = $100;
rcx = r15;
use rcx;
r15 = rcx;
rdi <<= r15;
call foo0(i32 a) -> i32;
r15 = rax;
rdx = r15;
r15 = rdx;
rdx = r15;
exit rdx;
BB5:
=== AFTER ===
BB2: fn foo0(i32 a) -> i32
rbx = rdi;
rdx = $100;
rax = rdi;
rax = rax + rdx;
rcx = rax;
return rax;
BB3:
BB4: start
rcx = $100;
use rcx;
r15 = $100;
rdi <<= $100;
call foo0(i32 a) -> i32;
exit rdx;
BB5:
/:
