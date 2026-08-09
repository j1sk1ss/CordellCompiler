: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu :
: BLOCK_TEST :
: RUN_ASM :

start() {
    i64 value;
    i32 size = sizeof(value) as i32;

    exit (size + 34) as u8;
}

:/ OUTPUT
@exit_code=42
/:
