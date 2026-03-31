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

:/ OUTPUT
BB1: start
jmp lb10;
BB2: lb12:
BB3: lb16:
jmp lb11;
BB4:
BB5: lb22:
jmp lb11;
BB6: lb10:
%1 = $1;
%0 = %1 < 1;
cmp %0, 0;
je lb29;
jne lb28;
BB7: lb28:
jmp lb22;
BB8: lb29:
%3 = $1;
%2 = %3 > 1;
cmp %2, 0;
je lb31;
jne lb30;
BB9: lb30:
%5 = $1;
%4 = %5 < 2;
cmp %4, 0;
je lb39;
jne lb38;
BB10: lb38:
jmp lb22;
BB11: lb39:
%7 = $1;
%6 = %7 > 2;
cmp %6, 0;
je lb41;
jne lb40;
BB12: lb40:
jmp lb22;
BB13: lb41:
jmp lb16;
BB14: lb31:
jmp lb12;
BB15: lb11:
send
/: