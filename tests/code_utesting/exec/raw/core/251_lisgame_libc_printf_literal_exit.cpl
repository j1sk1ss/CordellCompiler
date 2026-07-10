: BUG :
: BLOCK_TEST :
: ASM_ARCH=x86_64_nasm_macho :
: RUN_ASM[libs="System"] :

@[vname("_printf")]
@[abi]
extern function printf(ptr i8 fmt, ...) -> i32;

@[vname("_fflush")]
@[abi]
extern function fflush(ptr i0 stream) -> i32;

start() {
    printf(ref "Camera flash\n");
    fflush(0 as ptr i0);
    exit 42;
}

:/ OUTPUT
Camera flash
@exit_code=42
/:
