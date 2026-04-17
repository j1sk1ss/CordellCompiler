function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start() {
    ptr str a = ref "AB";
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
%1 = %4;
%2 = str_alloc(str(CD));
%5 = %1;
%6 = $0 * 1;
%7 = %5 + %6;
%8 = *(%7);
use %8;
stparam(%8);
call putc0(i8 c) -> i0;
%9 = %1;
%10 = $1 * 1;
%11 = %9 + %10;
%12 = *(%11);
use %12;
stparam(%12);
call putc0(i8 c) -> i0;
%13 = &(%2);
%14 = $0 * 1;
%16 = %14 as u64;
%15 = %13 + %16;
%17 = *(%15);
use %17;
stparam(%17);
call putc0(i8 c) -> i0;
%18 = &(%2);
%19 = $1 * 1;
%21 = %19 as u64;
%20 = %18 + %21;
%22 = *(%20);
use %22;
stparam(%22);
call putc0(i8 c) -> i0;
exit $0;
BB4:
/: