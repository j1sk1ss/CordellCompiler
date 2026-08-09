: BUG :
: BLOCK_TEST :
: ASM_ARCH=x86_64_nasm_macho :
: RUN_ASM[libs="System"] :

@[vname("_strdup")] @[abi]
extern function strdup(ptr i8 text) -> ptr i8;

start() {
    ptr i8 copy = strdup(ref "*");
    exit copy[0] as u8;
}

:/ OUTPUT
@exit_code=42
/:
