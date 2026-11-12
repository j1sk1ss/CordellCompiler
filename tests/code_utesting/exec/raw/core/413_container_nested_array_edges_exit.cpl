: BLOCK_TEST :
: RUN_ASM :

container row413 { arr values[4, i32]; }
container table413 { arr rows[3, row413]; }

start() {
    table413 table;
    table.rows[0].values[0] = 5;
    table.rows[0].values[3] = 7;
    table.rows[2].values[0] = 11;
    table.rows[2].values[3] = 19;
    exit (table.rows[0].values[0] + table.rows[0].values[3] + table.rows[2].values[0] + table.rows[2].values[3]) as u8;
}

:/ OUTPUT
@exit_code=42
/:
