function itoa(i32 num, ptr i8 buffer, i32 blen) -> i0 {
    if not num; {
        dref buffer = '0';
        return;
    }

    i32 i = 0;
    arr tmp[32, i8];
    while num > 0 as i32; {
        tmp[i] = '0' + (num % 10) as i8;
        num /= 10;
        i += 1;
    }

    i32 j = 0;
    while i > 0 && j < blen; {
        buffer[j] = tmp[i];
        j += 1;
        i -= 1;
    }
}

glob arr buffer[32, i8] = { 0 };

start() {
    itoa(123321, ref buffer, sizeof(buffer) as i32);
    syscall(0x2000004, 1, ref buffer, sizeof(buffer));
}

:/ OUTPUT
%6 = arr_alloc($32);
BB2: fn itoa0(i32 num, i8* buffer, i32 blen) -> i0
%0 = ldparam();
%1 = ldparam();
%2 = ldparam();
%7 = !%0;
cmp %7, 0;
je lb21;
jne lb19;
BB3: lb19:
*(%1) = $48;
return ;
BB4: lb21:
%8 = $0 as i32;
%3 = %8;
%4 = arr_alloc($32);
BB5: lb40:
%9 = $0 as i32;
%10 = %0 > %9;
cmp %10, 0;
je lb42;
jne lb41;
BB6: lb41:
%13 = $10 as i32;
%12 = %0 % %13;
%11 = %12 as i8;
%14 = $48 + %11;
%15 = &(%4);
%16 = %3 * 1;
%18 = %16 as u64;
%17 = %15 + %18;
*(%17) = %14;
%20 = $10 as i32;
%19 = %0 / %20;
%0 = %19;
%22 = $1 as i32;
%21 = %3 + %22;
%3 = %21;
jmp lb40;
BB7: lb42:
%23 = $0 as i32;
%5 = %23;
BB8: lb82:
%26 = $0 as i32;
%25 = %3 > %26;
cmp %25, 0;
je lb91;
jne lb90;
BB9: lb90:
%27 = %5 < %2;
%28 = %27 as i64;
%24 = %28;
jmp lb92;
BB10: lb91:
%24 = 0;
jmp lb92;
BB11: lb92:
cmp %24, 0;
je lb84;
jne lb83;
BB12: lb83:
%29 = &(%4);
%30 = %3 * 1;
%32 = %30 as u64;
%31 = %29 + %32;
%33 = *(%31);
%34 = %1;
%35 = %5 * 1;
%37 = %35 as u64;
%36 = %34 + %37;
*(%36) = %33;
%39 = $1 as i32;
%38 = %5 + %39;
%5 = %38;
%41 = $1 as i32;
%40 = %3 - %41;
%3 = %40;
jmp lb82;
BB13: lb84:
fend
BB14: start
%42 = &(%6);
use %42;
%43 = 32 as i32;
use %43;
stparam(%43);
stparam(%42);
stparam($123321);
call itoa0;
%44 = &(%6);
use %44;
stparam(32);
stparam(%44);
stparam($1);
stparam($33554436);
syscall;
send
/: