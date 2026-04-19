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
BB5: lb51:
%9 = $0 as i32;
%10 = %0 > %9;
cmp %10, 0;
je lb53;
jne lb52;
BB6: lb52:
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
jmp lb51;
BB7: lb53:
%23 = $0 as i32;
%5 = %23;
BB8: lb117:
%25 = $0 as i32;
%24 = %3 > %25;
cmp %24, 0;
je lb129;
jne lb128;
BB9: lb128:
%26 = %5 < %2;
%27 = %26;
jmp lb130;
BB10: lb129:
%27 = 0;
jmp lb130;
BB11: lb130:
cmp %27, 0;
je lb119;
jne lb118;
BB12: lb118:
%28 = &(%4);
%29 = %3 * 1;
%31 = %29 as u64;
%30 = %28 + %31;
%32 = *(%30);
%33 = %1;
%34 = %5 * 1;
%36 = %34 as u64;
%35 = %33 + %36;
*(%35) = %32;
%38 = $1 as i32;
%37 = %5 + %38;
%5 = %37;
%40 = $1 as i32;
%39 = %3 - %40;
%3 = %39;
jmp lb117;
BB13: lb119:
fend
BB14: start
%41 = &(%6);
use %41;
%42 = 32 as i32;
use %42;
stparam($123321);
stparam(%41);
stparam(%42);
call itoa0(i32 num, i8* buffer, i32 blen) -> i0;
%43 = &(%6);
use %43;
stparam($33554436);
stparam($1);
stparam(%43);
stparam(32);
syscall;
send
/: