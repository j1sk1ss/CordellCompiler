{
    @[entry]
    function foo() -> i0 {
        while 1; {
            break;
            while 1; {
                break;
            }
            break;
        }
    }
}

: OUTPUT
BB{X}: fn _main() -> i0
BB{X}: lb{X}:
cmp $1, 0;
je lb{X};
jne lb{X};
BB{X}: lb{X}:
jmp lb{X};
BB{X}: lb{X}:
cmp $1, 0;
je lb{X};
jne lb{X};
BB{X}: lb{X}:
jmp lb{X};
BB{X}:
BB{X}: lb{X}:
jmp lb{X};
BB{X}:
BB{X}: lb{X}:
fend
: