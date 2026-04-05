function itoa(i32 num, ptr i8 buffer, i32 blen) -> i0 {
    if not num; {
        dref buffer = '0';
        return;
    }

    i32 i = 0;
    arr tmp[32, i8];
    while num > 0; {
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
%0 = %44;
%1 = %45;
%2 = %46;
%47 = %0;
test %47, %47;
sete al;
%7 = al;
cmp %7, 0;
je lb21;
jne lb19;
BB3: lb19:
*(%1) = $48;
return ;
BB4: lb21:
%8 = $0;
%3 = %8;
%4 = arr_alloc($32);
BB5: lb51:
%10 = $0;
%48 = %0;
cmp %48, %10;
setg al;
%9 = al;
cmp %9, 0;
je lb53;
jne lb52;
BB6: lb52:
%13 = $10;
rdx = rdx ^ rdx;
%49 = %0;
edx = %49 % %13;
%12 = edx;
%11 = %12;
%50 = $48;
%50 = %50 + %11;
%14 = %50;
%15 = &(%4);
%51 = %3;
%51 = %51 * 1;
%16 = %51;
%18 = %16;
%52 = %15;
%52 = %52 + %18;
%17 = %52;
*(%17) = %14;
%20 = $10;
rdx = rdx ^ rdx;
%53 = %0;
%53 = %53 / %20;
%19 = %53;
%0 = %19;
%22 = $1;
%54 = %3;
%54 = %54 + %22;
%21 = %54;
%3 = %21;
jmp lb51;
BB7: lb53:
%23 = $0;
%5 = %23;
BB8: lb116:
%25 = $0;
%55 = %3;
cmp %55, %25;
setg al;
%24 = al;
cmp %24, 0;
je lb128;
jne lb127;
BB9: lb127:
%56 = %5;
cmp %56, %2;
setl al;
%26 = al;
%27 = %26;
jmp lb129;
BB10: lb128:
%27 = 0;
jmp lb129;
BB11: lb129:
cmp %27, 0;
je lb118;
jne lb117;
BB12: lb117:
%28 = &(%4);
%57 = %3;
%57 = %57 * 1;
%29 = %57;
%31 = %29;
%58 = %28;
%58 = %58 + %31;
%30 = %58;
%32 = *(%30);
%33 = %1;
%59 = %5;
%59 = %59 * 1;
%34 = %59;
%36 = %34;
%60 = %33;
%60 = %60 + %36;
%35 = %60;
*(%35) = %32;
%38 = $1;
%61 = %5;
%61 = %61 + %38;
%37 = %61;
%5 = %37;
%40 = $1;
%62 = %3;
%62 = %62 - %40;
%39 = %62;
%3 = %39;
jmp lb116;
BB13: lb118:
fend
BB14: start
%41 = &(%6);
use %41;
%42 = 32;
use %42;
%63 <<= $123321;
%64 <<= %41;
%65 <<= %42;
call itoa0(i32 num, i8* buffer, i32 blen) -> i0;
%43 = &(%6);
use %43;
%66 <<= $33554436;
push(rdi);
%67 <<= $1;
push(rsi);
%68 <<= %43;
push(rdx);
%69 <<= 32;
syscall;
rdx = pop();
rsi = pop();
rdi = pop();
send
/: