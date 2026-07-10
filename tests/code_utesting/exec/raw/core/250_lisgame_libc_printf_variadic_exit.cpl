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
    printf(ref "Scene %i/%i: %s\n", 2 as i32, 4 as i32, ref "Parking lot");
    printf(ref "Focus=%i Empathy=%i Storm=%i\n", 3 as i32, 5 as i32, 7 as i32);
    fflush(0 as ptr i0);
    exit 42;
}

:/ OUTPUT
Scene 2/4: Parking lot
Focus=3 Empathy=5 Storm=7
@exit_code=42
/:
