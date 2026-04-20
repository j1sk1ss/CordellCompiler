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
%51 = %13;
push(%52);
unknwn;
rdx = %50 % %51;
rax = rdx;
%52 = pop();
%12 = eax;
%11 = %12;
%53 = $48;
%55 = %55 + %11;
%14 = %54;
%15 = &(%4);
%56 = %3;
%58 = %58 * 1;
%16 = %57;
%18 movsxd %16;
%59 = %15;
%61 = %61 + %18;
%17 = %60;
*(%17) = %14;
%20 = $10;
%62 = %0;
%64 = %20;
push(%65);
unknwn;
%63 = %63 / %64;
%65 = pop();
%19 = eax;
%0 = %19;
%22 = $1;
%66 = %3;
%68 = %68 + %22;
%21 = %67;
%3 = %21;
jmp lb37;
BB7: lb39:
%23 = $0;
%5 = %23;
BB8: lb79:
%25 = $0;
%69 = %3;
cmp %69, %25;
setg al;
%24 = al;
cmp %24, 0;
je lb87;
jne lb86;
BB9: lb86:
%70 = %5;
cmp %70, %2;
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
%71 = %3;
%73 = %73 * 1;
%29 = %72;
%31 movsxd %29;
%74 = %28;
%76 = %76 + %31;
%30 = %75;
%32 = *(%30);
%33 = %1;
%77 = %5;
%79 = %79 * 1;
%34 = %78;
%36 movsxd %34;
%80 = %33;
%82 = %82 + %36;
%35 = %81;
*(%35) = %32;
%38 = $1;
%83 = %5;
%85 = %85 + %38;
%37 = %84;
%5 = %37;
%40 = $1;
%86 = %3;
%88 = %88 - %40;
%39 = %87;
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
%89 <<= $123321;
push(rsi);
%90 <<= %41;
push(rdx);
%91 <<= %42;
call itoa0(i32 num, i8* buffer, i32 blen) -> i0;
rdx = pop();
rsi = pop();
rdi = pop();
%43 = &(%6);
use %43;
%92 <<= $33554436;
push(rdi);
%93 <<= $1;
push(rsi);
%94 <<= %43;
push(rdx);
%95 <<= 32;
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