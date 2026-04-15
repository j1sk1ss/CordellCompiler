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
rdx = $100;
use rdx;
push(rdi);
rdi <<= rdx;
call foo0(i32 a) -> i32;
rdi = pop();
rcx = rax;
rdi = rcx;
exit rdi;
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
rdx = $100;
use rdx;
push(rdi);
rdi <<= $100;
call foo0(i32 a) -> i32;
rdi = pop();
rcx = rax;
rdi = rax;
exit rdi;
BB5:
/: