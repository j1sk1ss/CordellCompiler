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
je lb18;
jne lb16;
BB3: lb16:
*(%1) = $48;
return ;
BB4: lb18:
%8 = $0;
%3 = %8;
%4 = arr_alloc($32);
BB5: lb37:
%10 = $0;
%48 = %0;
cmp %48, %10;
setg al;
%9 = al;
cmp %9, 0;
je lb39;
jne lb38;
BB6: lb38:
%13 = $10;
%49 = %0;
%50 = %13;
push(%51);
unknwn;
edx = %49 % %50;
eax = edx;
%51 = pop();
%12 = eax;
%11 = %12;
%52 = $48;
%53 = %52 + %11;
%14 = %53;
%15 = &(%4);
%54 = %3;
%55 = %54 * 1;
%16 = %55;
%18 movsxd %16;
%56 = %15;
%57 = %56 + %18;
%17 = %57;
*(%17) = %14;
%20 = $10;
%58 = %0;
%59 = %20;
push(%60);
unknwn;
%61 = %58 / %59;
%60 = pop();
%19 = %61;
%0 = %19;
%22 = $1;
%62 = %3;
%63 = %62 + %22;
%21 = %63;
%3 = %21;
jmp lb37;
BB7: lb39:
%23 = $0;
%5 = %23;
BB8: lb79:
%25 = $0;
%64 = %3;
cmp %64, %25;
setg al;
%24 = al;
cmp %24, 0;
je lb87;
jne lb86;
BB9: lb86:
%65 = %5;
cmp %65, %2;
setl al;
%26 = al;
%27 = %26;
jmp lb88;
BB10: lb87:
%27 = 0;
jmp lb88;
BB11: lb88:
cmp %27, 0;
je lb81;
jne lb80;
BB12: lb80:
%28 = &(%4);
%66 = %3;
%67 = %66 * 1;
%29 = %67;
%31 movsxd %29;
%68 = %28;
%69 = %68 + %31;
%30 = %69;
%32 = *(%30);
%33 = %1;
%70 = %5;
%71 = %70 * 1;
%34 = %71;
%36 movsxd %34;
%72 = %33;
%73 = %72 + %36;
%35 = %73;
*(%35) = %32;
%38 = $1;
%74 = %5;
%75 = %74 + %38;
%37 = %75;
%5 = %37;
%40 = $1;
%76 = %3;
%77 = %76 - %40;
%39 = %77;
%3 = %39;
jmp lb79;
BB13: lb81:
fend
BB14: start
%41 = &(%6);
use %41;
%42 = 32;
use %42;
push(rdi);
%78 <<= $123321;
push(rsi);
%79 <<= %41;
push(rdx);
%80 <<= %42;
call itoa0(i32 num, i8* buffer, i32 blen) -> i0;
rdx = pop();
rsi = pop();
rdi = pop();
%43 = &(%6);
use %43;
%81 <<= $33554436;
push(rdi);
%82 <<= $1;
push(rsi);
%83 <<= %43;
push(rdx);
%84 <<= 32;
push(rcx);
push(r11);
syscall;
r11 = pop();
rcx = pop();
rdx = pop();
rsi = pop();
rdi = pop();
send
/: