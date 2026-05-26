start(i32 argc) {
    if argc == 1; {
        exit 1;
    }
    else if argc == 2; {
        exit 2;
    }
    else {
        exit 3;
    }
}

:/ OUTPUT
BB1: start
%0 = strt_loadarg();
%2 = $1 as i32;
%1 = %0 == %2;
cmp %1, 0;
je lb11;
jne lb10;
BB2: lb10:
%3 = $1 as u8;
exit %3;
BB3: lb11:
%5 = $2 as i32;
%4 = %0 == %5;
cmp %4, 0;
je lb25;
jne lb24;
BB4: lb24:
%6 = $2 as u8;
exit %6;
BB5: lb25:
%7 = $3 as u8;
exit %7;
BB6: lb26:
jmp lb12;
BB7: lb12:
send
/: