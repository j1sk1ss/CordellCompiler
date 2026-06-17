: BLOCK_TEST :
: RUN_ASM :

container meter {
    i32 value;
}

glob meter G;

start() {
    G.value = 42;
    exit G.value as u8;
}

:/ OUTPUT
@exit_code=42
/:
