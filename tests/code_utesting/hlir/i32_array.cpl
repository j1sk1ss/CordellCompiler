function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function putdigit(i32 x) -> i0 {
    i8 c = x as i8;
    c += 48 as i8;
    putc(c);
}

glob arr a[16, i32];

start(i32 argc, ptr ptr i8 argv) {
    a[2] = 7;
    a[7] = 3;
    putdigit(a[7]);
    putdigit(a[2]);
    exit 0;
}

:/ OUTPUT
%3 = arr_alloc($16);
BB3: fn putc0(i8 c) -> i0
%0 = ldparam();
%6 = &(%0);
use %6;
stparam($33554436);
stparam($1);
stparam(%6);
stparam($1);
syscall;
fend
BB4: fn putdigit1(i32 x) -> i0
%1 = ldparam();
%7 = %1 as i8;
%2 = %7;
%8 = $48 as i8;
%9 = %2 + %8;
%2 = %9;
use %2;
stparam(%2);
call putc0(i8 c) -> i0;
fend
BB5: start
%4 = strt_loadarg();
%5 = strt_loadarg();
%10 = &(%3);
%11 = $2 * 4;
%13 = %11 as u64;
%12 = %10 + %13;
%14 = $7 as i32;
*(%12) = %14;
%15 = &(%3);
%16 = $7 * 4;
%18 = %16 as u64;
%17 = %15 + %18;
%19 = $3 as i32;
*(%17) = %19;
%20 = &(%3);
%21 = $7 * 4;
%23 = %21 as u64;
%22 = %20 + %23;
%24 = *(%22);
use %24;
stparam(%24);
call putdigit1(i32 x) -> i0;
%25 = &(%3);
%26 = $2 * 4;
%28 = %26 as u64;
%27 = %25 + %28;
%29 = *(%27);
use %29;
stparam(%29);
call putdigit1(i32 x) -> i0;
exit $0;
BB6:
/: