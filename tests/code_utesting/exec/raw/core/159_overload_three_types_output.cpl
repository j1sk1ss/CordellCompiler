: ASM_ARCH=x86_64_nasm_macho :
: BLOCK_TEST :
: RUN_ASM :

function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function emit(i8 x) -> i0 {
    putc(('A' + x) as i8);
}

function emit(i32 x) -> i0 {
    putc(('a' + x) as i8);
}

function emit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

start() {
    emit(1 as i8);
    emit(2 as i32);
    emit(3 as i64);
    exit 0;
}

:/ OUTPUT
Bc3
/:
