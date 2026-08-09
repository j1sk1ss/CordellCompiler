: BLOCK_TEST :
: RUN_ASM :

container cell {
    i32 value;
}

glob arr CELLS[3, cell];

start() {
    CELLS[0].value = 10;
    CELLS[1].value = 14;
    CELLS[2].value = 18;

    exit (CELLS[0].value + CELLS[1].value + CELLS[2].value) as u8;
}

:/ OUTPUT
@exit_code=42
/:
