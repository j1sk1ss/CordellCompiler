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
[SSA] %0 = %45;
[SSA] %1 = %46;
[SSA] %2 = %47;
%48 = %0;
test %48, %48;
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
BB5: lb40:
%10 = $0;
%49 = %0;
cmp %49, %10;
setg al;
%9 = al;
cmp %9, 0;
je lb42;
jne lb41;
BB6: lb41:
%13 = $10;
%50 = %0;
%51 = %13;
push(%52);
unknwn;
edx = %50 % %51;
eax = edx;
%52 = pop();
%12 = eax;
%11 = %12;
%53 = $48;
%54 = %53 + %11;
%14 = %54;
%15 = &(%4);
%55 = %3;
%56 = %55 * 1;
%16 = %56;
%18 movsxd %16;
%57 = %15;
%58 = %57 + %18;
%17 = %58;
*(%17) = %14;
%20 = $10;
%59 = %0;
%60 = %20;
push(%61);
unknwn;
%62 = %59 / %60;
%61 = pop();
%19 = %62;
%0 = %19;
%22 = $1;
%63 = %3;
%64 = %63 + %22;
%21 = %64;
%3 = %21;
jmp lb40;
BB7: lb42:
%23 = $0;
%5 = %23;
BB8: lb82:
%26 = $0;
%65 = %3;
cmp %65, %26;
setg al;
%25 = al;
cmp %25, 0;
je lb91;
jne lb90;
BB9: lb90:
%66 = %5;
cmp %66, %2;
setl al;
%27 = al;
%28 movsxd %27;
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
%67 = %3;
%68 = %67 * 1;
%30 = %68;
%32 movsxd %30;
%69 = %29;
%70 = %69 + %32;
%31 = %70;
%33 = *(%31);
%34 = %1;
%71 = %5;
%72 = %71 * 1;
%35 = %72;
%37 movsxd %35;
%73 = %34;
%74 = %73 + %37;
%36 = %74;
*(%36) = %33;
%39 = $1;
%75 = %5;
%76 = %75 + %39;
%38 = %76;
%5 = %38;
%41 = $1;
%77 = %3;
%78 = %77 - %41;
%40 = %78;
%3 = %40;
jmp lb82;
BB13: lb84:
fend
BB14: start
%42 = &(%6);
use %42;
%43 = 32;
use %43;
push(rdx);
%79 <<= %43;
push(rsi);
%80 <<= %42;
push(rdi);
%81 <<= $123321;
call itoa0(i32 num, i8* buffer, i32 blen) -> i0;
rdi = pop();
rsi = pop();
rdx = pop();
%44 = &(%6);
use %44;
push(rdx);
%82 <<= 32;
push(rsi);
%83 <<= %44;
push(rdi);
%84 <<= $1;
%85 <<= $33554436;
push(rcx);
push(r11);
syscall;
r11 = pop();
rcx = pop();
rdi = pop();
rsi = pop();
rdx = pop();
send
/: