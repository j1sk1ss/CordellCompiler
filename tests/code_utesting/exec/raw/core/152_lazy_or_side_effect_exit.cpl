: ASM_ARCH=x86_64_nasm_macho :
: BLOCK_TEST :
: RUN_ASM :

glob i32 counter = 0;

function mark() -> i32 {
    counter += 1;
    return 0;
}

start() {
    if 1 || mark(); {
        counter += 2;
    }

    exit counter as u8;
}

:/ OUTPUT
@exit_code=2
/:
