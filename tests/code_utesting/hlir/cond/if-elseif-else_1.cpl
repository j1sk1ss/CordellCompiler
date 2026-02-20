{
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
}

: OUTPUT
BB{X}: start
%0 = strt_loadarg();
%2 = %0 as i64;
%1 = %2 == $1;
cmp %1, 0;
je lb{X};
jne lb{X};
BB{X}: lb{X}:
exit $1;
BB{X}: lb{X}:
%4 = %0 as i64;
%3 = %4 == $2;
cmp %3, 0;
je lb{X};
jne lb{X};
BB4: lb{X}:
exit $2;
BB{X}: lb{X}:
exit $3;
BB{X}: lb{X}:
jmp lb{X};
BB{X}: lb{X}:
send
: