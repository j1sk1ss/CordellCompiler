: RUN_ASM :

container row {
    arr cells[6, i8];
}

start() {
    row r;
    i32 i = 0;

    while i < 6; {
        r.cells[i] = (i + 1) as i8;
        i += 1;
    }

    r.cells[2] += 10;
    r.cells[5] += 20;

    exit (r.cells[0] + r.cells[2] + r.cells[5]) as u8;
}

:/ OUTPUT
@exit_code=40
/:
