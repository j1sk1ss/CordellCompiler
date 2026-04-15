function foo(i32 a, i32 b, i32 c, i32 d, i32 e, i32 f, i32 g, i32 k, i32 l) {
}

start() {
    foo(1, 2, 3, 4, 5, 6, 7, 8, 9);
}

:/ OUTPUT
BB2: fn foo0(i32 a, i32 b, i32 c, i32 d, i32 e, i32 f, i32 g, i32 k, i32 l)
rcx = rdi;
rcx = rsi;
rcx = rdx;
rcx = rcx;
rcx = r8;
rcx = r9;
rcx = [rbp + 8];
rcx = [rbp + 16];
rcx = [rbp + 24];
fend
BB3: start
push(rdi);
rdi <<= $1;
push(rsi);
rsi <<= $2;
push(rdx);
rdx <<= $3;
push(rcx);
rcx <<= $4;
push(r8);
r8 <<= $5;
push(r9);
r9 <<= $6;
push($7);
push($8);
push($9);
call foo0(i32 a, i32 b, i32 c, i32 d, i32 e, i32 f, i32 g, i32 k, i32 l);
r9 = pop();
r8 = pop();
rcx = pop();
rdx = pop();
rsi = pop();
rdi = pop();
send
/: