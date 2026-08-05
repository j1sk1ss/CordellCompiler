: BLOCK_TEST :
: RUN_ASM :

container aligned_box {
    i32 value;
}

start() {
    @[align(32)] aligned_box b;
    b.value = 42;

    exit b.value as u8;
}

:/ OUTPUT
@exit_code=42
/:
