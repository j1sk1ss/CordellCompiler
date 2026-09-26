: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu :
: BLOCK_TEST :
: RUN_ASM :

@[like_c]
container pair398 {
    i32 left;
    i32 right;
}

@[like_c]
container block398 {
    arr pairs[2, pair398];
    i64 tail;
}

start() {
    exit (sizeof(block398) + sizeof(pair398) + 10) as u8;
}

:/ OUTPUT
@exit_code=42
/:
