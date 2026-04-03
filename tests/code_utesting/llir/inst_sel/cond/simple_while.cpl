start(i32 a) {
    while a; {
        a += 1 as i32;
    }
}

:/ OUTPUT
BB1: start
%0 = [rbp + 8];
BB2: lb13:
cmp %0, 0;
je lb15;
jne lb14;
BB3: lb14:
%1 = $1;
%3 = %0;
%3 = %3 + %1;
%2 = %3;
%0 = %2;
jmp lb13;
BB4: lb15:
send
/: