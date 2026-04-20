start(i32 a) {
    while a; {
        a += 1 as i32;
    }
}

:/ OUTPUT
BB1: start
%0 = %3;
BB2: lb10:
cmp %0, 0;
je lb12;
jne lb11;
BB3: lb11:
%1 = $1;
%4 = %0;
%6 = %6 + %1;
%2 = %5;
%0 = %2;
jmp lb10;
BB4: lb12:
send
/: