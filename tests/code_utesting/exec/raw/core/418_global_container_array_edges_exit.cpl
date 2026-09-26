: BLOCK_TEST :
: RUN_ASM :

container cell418 { i32 value; }
glob arr cells418[5, cell418];

start() {
    cells418[0].value = 5;
    cells418[2].value = 13;
    cells418[4].value = 24;
    exit (cells418[0].value + cells418[2].value + cells418[4].value) as u8;
}

:/ OUTPUT
@exit_code=42
/:
