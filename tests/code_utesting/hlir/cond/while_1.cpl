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
BB{X}: lb4:
cmp $1, 0;
je lb6;
jne lb5;
BB{X}: lb5:
jmp lb6;
BB{X}: lb9:
cmp $1, 0;
je lb11;
jne lb10;
BB{X}: lb10:
jmp lb11;
BB{X}:
BB{X}: lb11:
jmp lb6;
BB{X}:
BB{X}: lb6:
fend
: