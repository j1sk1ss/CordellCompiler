function foo(i32 a) -> i32 {
    return a + 100 as i32;
}

start() {
    exit foo(100 as i32);
}

:/ OUTPUT
=== BEFORE ===
BB2: fn foo0(i32 a) -> i32
rdx = rdi;
rbx = $100;
rax = rdx;
rax = rax + rbx;
rcx = rax;
rax = rcx;
return rax;
BB3:
BB4: start
rcx = $100;
use rcx;
rdi <<= rcx;
call foo0(i32 a) -> i32;
rdx = rax;
rdx = rdx;
exit rdx;
BB5:
=== AFTER ===
BB2: fn foo0(i32 a) -> i32
rdx = rdi;
rbx = $100;
rax = rdi;
rax = rax + rbx;
rcx = rax;
return rax;
BB3:
BB4: start
rcx = $100;
use rcx;
rdi <<= $100;
call foo0(i32 a) -> i32;
rdx = rax;
exit rdx;
BB5:
/: