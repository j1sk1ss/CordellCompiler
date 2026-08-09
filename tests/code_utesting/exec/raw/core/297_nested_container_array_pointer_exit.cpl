: BLOCK_TEST :
: RUN_ASM :

container row {
    arr values[3, i32];
}

container table {
    arr rows[2, row];
}

start() {
    table t;
    ptr i32 cell = ref t.rows[1].values[2];

    dref cell = 42;

    exit t.rows[1].values[2] as u8;
}

:/ OUTPUT
@exit_code=42
/:
