function foo(i32 a, i32 b, i32 c, i32 d, i32 e, i32 f, i32 g, i32 k, i32 l) {
}

start() {
    foo(1, 2, 3, 4, 5, 6, 7, 8, 9);
}

:/ OUTPUT
BB2: fn foo0(i32 a, i32 b, i32 c, i32 d, i32 e, i32 f, i32 g, i32 k, i32 l)
rax = rdi;
rax = rsi;
rax = rdx;
rax = rcx;
rax = r8;
rax = r9;
rax = [rbp + 8];
rax = [rbp + 16];
rax = [rbp + 24];
fend
BB3: start
rdi <<= $1;
rsi <<= $2;
rdx <<= $3;
rcx <<= $4;
r8 <<= $5;
r9 <<= $6;
push($7);
push($8);
push($9);
call foo0(i32 a, i32 b, i32 c, i32 d, i32 e, i32 f, i32 g, i32 k, i32 l);
send
/: