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
jmp lb7;
BB2: lb9:
BB3: lb13:
jmp lb8;
BB4:
BB5: lb17:
jmp lb8;
BB6: lb7:
%1 = $1;
%0 = %1 < 1;
cmp %0, 0;
je lb24;
jne lb23;
BB7: lb23:
jmp lb17;
BB8: lb24:
%3 = $1;
%2 = %3 > 1;
cmp %2, 0;
je lb26;
jne lb25;
BB9: lb25:
%5 = $1;
%4 = %5 < 2;
cmp %4, 0;
je lb33;
jne lb32;
BB10: lb32:
jmp lb17;
BB11: lb33:
%7 = $1;
%6 = %7 > 2;
cmp %6, 0;
je lb35;
jne lb34;
BB12: lb34:
jmp lb17;
BB13: lb35:
jmp lb13;
BB14: lb26:
jmp lb9;
BB15: lb8:
send
/: