: BLOCK_TEST :
: RUN_ASM :

container item {
    i32 value;
}

start() {
    arr items[3, item];

    items[0].value = 7;
    items[1].value = 14;
    items[2].value = 21;

    exit (items[0].value + items[1].value + items[2].value) as u8;
}

:/ OUTPUT
@exit_code=42
/:
