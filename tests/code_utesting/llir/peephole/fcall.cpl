function foo(i32 a) -> i32 {
    return a + 100;
}

start() {
    exit foo(100);
}

: OUTPUT
=== BEFORE ===
BB{X} fn foo0(i32 a) -> i32
r15 = rdi;
rbx = r15;
rdx movsxd rbx;
r15 = rdx;
rax = r15;
rax = rax + $100;
r15 = rax;
rcx = r15;
r15 = rcx;
rax = r15;
return rax;
BB{X}
BB{X} start
use $100;
r15 = $100;
rdi <<= r15;
call foo0(i32 a) -> i32;
r15 = rax;
rcx = r15;
r15 = rcx;
rdx = r15;
exit rdx;
BB{X}
=== AFTER ===
BB{X} fn foo0(i32 a) -> i32
rbx = rdi;
rdx movsxd rdi;
rax = rdi;
rax = rax + $100;
rcx = rax;
return rax;
BB{X}
BB{X} start
use $100;
r15 = $100;
rdi <<= $100;
call foo0(i32 a) -> i32;
rcx = rax;
rdx = rax;
exit rdx;
BB{X}
: