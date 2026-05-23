function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start() {
    ptr i8 a = ref "AB";
    str b = "CD";

    putc(a[0]);
    putc(a[1]);
    putc(b[0]);
    putc(b[1]);
    exit 0;
}

:/ OUTPUT
BB2: fn putc0(i8 c) -> i0
%0 = ldparam();
%3 = &(%0);
use %3;
stparam($1);
stparam(%3);
stparam($1);
stparam($33554436);
syscall;
fend
BB3: start
%4 = &(str(AB));
%1 = %4;
%2 = str_alloc(str(CD));
%5 = %1;
%6 = $0 * 1;
%8 = %6 as u64;
%7 = %5 + %8;
%9 = *(%7);
use %9;
stparam(%9);
call putc0;
%10 = %1;
%11 = $1 * 1;
%13 = %11 as u64;
%12 = %10 + %13;
%14 = *(%12);
use %14;
stparam(%14);
call putc0;
%15 = &(%2);
%16 = $0 * 1;
%18 = %16 as u64;
%17 = %15 + %18;
%19 = *(%17);
use %19;
stparam(%19);
call putc0;
%20 = &(%2);
%21 = $1 * 1;
%23 = %21 as u64;
%22 = %20 + %23;
%24 = *(%22);
use %24;
stparam(%24);
call putc0;
%25 = $0 as u8;
exit %25;
BB4: send
/: