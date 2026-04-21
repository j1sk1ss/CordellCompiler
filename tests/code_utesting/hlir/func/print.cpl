function strlen(ptr i8 s) -> i32 {
    i32 l = 0;
    while dref s; {
        l += 1 as i32;
        s += 1 as ptr i8;
    }

    return l;
}

start(i32 argc, ptr ptr i8 argv) {
    syscall(0x2000004, 1, argv[1 as i32], strlen(argv[1 as i32]));
}

:/ OUTPUT
BB2: fn strlen0(i8* s) -> i32
%0 = ldparam();
%4 = $0 as i32;
%1 = %4;
BB3: lb15:
%5 = *(%0);
cmp %5, 0;
je lb17;
jne lb16;
BB4: lb16:
%6 = $1 as i32;
%7 = %1 + %6;
%1 = %7;
%8 = $1 as u64;
%9 = %0 + %8;
%0 = %9;
jmp lb15;
BB5: lb17:
return %1;
BB6:
BB7: start
%2 = strt_loadarg();
%3 = strt_loadarg();
%10 = $1 as i32;
%11 = %3;
%12 = %10 * 8;
%14 = %12 as u64;
%13 = %11 + %14;
%15 = *(%13);
use %15;
%16 = $1 as i32;
%17 = %3;
%18 = %16 * 8;
%20 = %18 as u64;
%19 = %17 + %20;
%21 = *(%19);
use %21;
stparam(%21);
call strlen0(i8* s) -> i32;
%22 = fret();
use %22;
stparam($33554436);
stparam($1);
stparam(%15);
stparam(%22);
syscall;
send
/: