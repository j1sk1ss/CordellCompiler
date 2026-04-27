start(i32 a) {
    while a; {
        a += 1 as i32;
    }
}

:/ OUTPUT
BB1: start
%0 = [rbp + 8];
BB2: lb10:
cmp %0, 0;
je lb12;
jne lb11;
BB3: lb11:
%1 = $1;
%3 = %0;
%4 = %3 + %1;
%2 = %4;
%0 = %2;
jmp lb10;
BB4: lb12:
send
/: