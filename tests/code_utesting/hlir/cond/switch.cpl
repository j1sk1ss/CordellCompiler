{
    start() {
        switch 1; {
            case 1; {
            }
            case 2; {
                break;
            }
            default {
            }
        }
    }
}

: OUTPUT
BB{X}: start
jmp lb{X};

BB{X}: lb{X}:

BB{X}: lb{X}:
jmp lb{X};

BB{X}: 
BB{X}: lb{X}:
jmp lb{X};

BB{X}: lb{X}:
%0 = $1 < 1;
cmp %0, 0;
je lb{X};
jne lb{X};

BB{X}: lb{X}:
jmp lb{X};

BB{X}: lb{X}:
%1 = $1 > 1;
cmp %1, 0;
je lb{X};
jne lb{X};

BB{X}: lb{X}:
%2 = $1 < 2;
cmp %2, 0;
je lb{X};
jne lb{X};

BB{X}: lb{X}:
jmp lb{X};

BB{X}: lb{X}:
%3 = $1 > 2;
cmp %3, 0;
je lb{X};
jne lb{X};

BB{X}: lb{X}:
jmp lb{X};

BB{X}: lb{X}:
jmp lb{X};

BB{X}: lb{X}:
jmp lb{X};

BB{X}: lb{X}:
send
: