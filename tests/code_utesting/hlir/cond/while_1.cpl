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

:/ OUTPUT
BB1: fn _main() -> i0
BB2: lb9:
cmp $1, 0;
je lb11;
jne lb10;
BB3: lb10:
jmp lb11;
BB4:
BB5: lb16:
cmp $1, 0;
je lb18;
jne lb17;
BB6: lb17:
jmp lb18;
BB7: jmp lb16;
BB8: lb18:
jmp lb11;
BB9: jmp lb9;
BB10: lb11:
fend
/: