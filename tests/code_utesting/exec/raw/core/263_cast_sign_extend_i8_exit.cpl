: BLOCK_TEST :
: RUN_ASM :

start() {
    i8 small = -1;
    i32 wide = small as i32;

    exit (wide + 43) as u8;
}

:/ OUTPUT
@exit_code=42
/:
