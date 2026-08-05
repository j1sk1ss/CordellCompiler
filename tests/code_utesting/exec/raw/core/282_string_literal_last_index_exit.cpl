: BLOCK_TEST :
: RUN_ASM :

start() {
    ptr i8 text = ref "012345*";

    exit text[6] as u8;
}

:/ OUTPUT
@exit_code=42
/:
