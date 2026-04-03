function foo(i32 a) {
    i32 b = a;
    i32 c = b + a + 10 as i32;
    return a + c;
}

start() {
    i32 a = 1 as i32;
    foo(a);
    i32 b = a + 10 as i32;
    exit b as i8;
}

:/ OUTPUT
BB2: fn foo0(i32 a)
r10 = rdi;
r9 = r10;
rax = r9;
rax = rax + r10;
rdx = rax;
r8 = $10;
rax = rdx;
rax = rax + r8;
rcx = rax;
rsi = rcx;
rax = r10;
rax = rax + rsi;
rbx = rax;
rax = rbx;
return rax;
BB3:
BB4: start
r9 = $1;
rsi = r9;
use rsi;
rdi <<= rsi;
push(r8);
push(rsi);
push(r10);
push(rax);
push(rdx);
push(rcx);
call foo0(i32 a);
rcx = pop();
rdx = pop();
rax = pop();
r10 = pop();
rsi = pop();
r8 = pop();
rcx = $10;
rax = rsi;
rax = rax + rcx;
r8 = rax;
r10 = r8;
rdx = r10;
rdx = rdx;
exit rdx;
BB5:
/: