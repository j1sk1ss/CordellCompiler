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
stparam($33554436);
stparam($1);
stparam(%3);
stparam($1);
syscall;
fend
BB3: start
%4 = &(str(AB));
%5 = %4 as u64;
%1 = %5;
%2 = str_alloc(str(CD));
%6 = %1;
%7 = $0 * 1;
%9 = %7 as u64;
%8 = %6 + %9;
%10 = *(%8);
use %10;
stparam(%10);
call putc0(i8 c) -> i0;
%11 = %1;
%12 = $1 * 1;
%14 = %12 as u64;
%13 = %11 + %14;
%15 = *(%13);
use %15;
stparam(%15);
call putc0(i8 c) -> i0;
%16 = &(%2);
%17 = $0 * 1;
%19 = %17 as u64;
%18 = %16 + %19;
%20 = *(%18);
use %20;
stparam(%20);
call putc0(i8 c) -> i0;
%21 = &(%2);
%22 = $1 * 1;
%24 = %22 as u64;
%23 = %21 + %24;
%25 = *(%23);
use %25;
stparam(%25);
call putc0(i8 c) -> i0;
exit $0;
BB4:
/: