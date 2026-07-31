: ASM_ARCH=x86_64_nasm_gnu :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(1, 1, ref c, 1);
}

function set(ptr i8 p) -> i0 {
    dref p = 'Z';
}

start() {
    i8 ch = 0;
    set(ref ch);
    putc(ch);
    exit 0;
}

:/ OUTPUT
@measure_time
@measure_lines
@repeat=5
Z
/:
