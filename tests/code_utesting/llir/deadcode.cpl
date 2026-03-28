start() {
    if 1; {
        exit 0;
    }
    else {
        exit 1;
    }

    exit 2;
}

:/ OUTPUT
BB{X} start
cmp $1, 0;
je lb{X}
jne lb{X}
BB{X} lb{X}
exit $0;
BB{X} lb{X}
exit $1;
BB{X} lb{X}
BB{X}
/:
