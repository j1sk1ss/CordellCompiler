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
je lb14;
jne lb13;
BB2: lb13:
exit $1;
BB3: lb14:
%4 = $2 as i32;
%3 = %0 == %4;
cmp %3, 0;
je lb33;
jne lb32;
BB4: lb32:
exit $2;
BB5: lb33:
exit $3;
BB6: lb34:
BB7: lb15:
/: