start() {
    if 1; {
        exit 0;
    }
    else {
        exit 1;
    }

    exit 2;
}

: OUTPUT
BB1: start
cmp $1, 0;
je lb5;
jne lb4;
BB2: lb4:
exit $0;
BB3: lb5:
exit $1;
BB4: lb6:
BB5:
: