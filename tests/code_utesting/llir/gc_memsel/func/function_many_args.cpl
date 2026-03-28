function foo(i32 a, i32 b, i32 c, i32 d, i32 e, i32 f, i32 g, i32 k, i32 l) {
}

start() {
    foo(1, 2, 3, 4, 5, 6, 7, 8, 9);
}

:/ OUTPUT
BB{X} fn foo0(i32 a, i32 b, i32 c, i32 d, i32 e, i32 f, i32 g, i32 k, i32 l)
r15 = rdi;
rax = r15;
r15 = rsi;
rax = r15;
r15 = rdx;
rax = r15;
r15 = rcx;
rax = r15;
r15 = r8;
rax = r15;
r15 = r9;
rax = r15;
r15 = [rbp + 8];
rax = r15;
r15 = [rbp + 16];
rax = r15;
r15 = [rbp + 24];
rax = r15;
fend
BB{X} start
r15 = $1;
rdi <<= r15;
r15 = $2;
rsi <<= r15;
r15 = $3;
rdx <<= r15;
r15 = $4;
rcx <<= r15;
r15 = $5;
r8 <<= r15;
r15 = $6;
r9 <<= r15;
push($7);
push($8);
push($9);
call foo0(i32 a, i32 b, i32 c, i32 d, i32 e, i32 f, i32 g, i32 k, i32 l);
send
/:
