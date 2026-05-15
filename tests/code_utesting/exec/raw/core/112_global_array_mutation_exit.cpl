: RUN_ASM :

glob arr values[4, i32] = { 1, 2, 3, 4 };

start() {
    values[1] = 9;
    exit (values[0] + values[1] + values[2] + values[3]) as u8;
}

:/ OUTPUT
@exit_code=17
/:
