{
    start() {
        i32 a = 0;
        ptr i32 b = ref a;
    }
}

: OUTPUT
BB{X}: start
rcx = $0;
rbx = rcx;
rsi = rbx;
[rbp - 8] = rsi;
rax = &([rbp - 8]);
rdi = rax;
rbp = rdi;
rdx = rbp;
[unused] kill(8);
[unused] kill(9);
kill(4);
[unused] kill(7);
[unused] kill(3);
[unused] kill(5);
[unused] kill(6);
[unused] kill(2);
send
: