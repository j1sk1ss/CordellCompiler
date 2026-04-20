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
exit $1;
BB3: lb11:
%4 = $2 as i32;
%3 = %0 == %4;
cmp %3, 0;
je lb24;
jne lb23;
BB4: lb23:
exit $2;
BB5: lb24:
exit $3;
BB6: lb25:
BB7: lb12:
/: