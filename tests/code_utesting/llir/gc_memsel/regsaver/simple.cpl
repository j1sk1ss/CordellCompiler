{
    function foo(i32 a) {
        i32 b = a;
        i32 c = b + a + 10;
        return a + c;
    }

    start() {
        i32 a = 1;
        foo(a);
        i32 b = a + 10;
        exit b;
    }
}

: OUTPUT
BB2: fn foo0(i32 a)
r15 = rdi;
r10 = r15;
r15 = r10;
rbx = r15;
r15 = rbx;
rax = r15;
rax = rax + r10;
r15 = rax;
rsi = r15;
rcx movsxd rsi;
r15 = rcx;
rax = r15;
rax = rax + $10;
r15 = rax;
r9 = r15;
r15 = r9;
r8 = r15;
r15 = r8;
r11 = r15;
r15 = r10;
rax = r15;
rax = rax + r11;
r15 = rax;
rdx = r15;
r15 = rdx;
rax = r15;
return rax;
BB3:
BB4: start
r15 = $1;
rcx = r15;
r15 = rcx;
r10 = r15;
use r10;
r15 = r10;
rdi <<= r15;
push(r8);
push(r9);
push(rsi);
push(r10);
push(rdx);
push(rax);
push(r15);
call foo0(i32 a);
r15 = pop();
rax = pop();
rdx = pop();
r10 = pop();
rsi = pop();
r9 = pop();
r8 = pop();
rdx movsxd r10;
r15 = rdx;
rax = r15;
rax = rax + $10;
r15 = rax;
r9 = r15;
r15 = r9;
rsi = r15;
r15 = rsi;
r8 = r15;
r15 = r8;
rdx = r15;
exit rdx;
BB5:
: