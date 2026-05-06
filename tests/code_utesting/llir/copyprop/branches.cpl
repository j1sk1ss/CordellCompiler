start() {
    i32 a = 1;
    i32 b;
    switch a; {
        case 1; {
            b = 10;
            i32 c = b + 10;
            exit c;
        }
        default {
            exit b;
        }
    }
}

:/ OUTPUT
BB1: start
%12 = $1;
jmp lb14;
BB2: lb16:
%5 = $10 + $10;
exit %5;
BB3: lb32:
exit %1;
BB4: lb14:
%7 = 1 as i64;
%9 = %12 as i64;
%8 = %9 < %7;
cmp %8, 0;
je lb40;
jne lb39;
BB5: lb39:
jmp lb32;
BB6: lb40:
%11 = %12 as i64;
%10 = %11 > %7;
cmp %10, 0;
je lb42;
jne lb41;
BB7: lb41:
jmp lb32;
BB8: lb42:
jmp lb16;
BB9: lb15:
send
/: