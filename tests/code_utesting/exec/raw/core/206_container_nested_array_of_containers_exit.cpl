: BLOCK_TEST :
: RUN_ASM :

container row {
    arr cells[3, i32];
}

container table {
    arr rows[2, row];

    @[self]
    function diag(ptr table self) -> i32 {
        return self.rows[0].cells[0] + self.rows[1].cells[1];
    }
}

start() {
    table t;
    t.rows[0].cells[0] = 12;
    t.rows[0].cells[1] = 99;
    t.rows[1].cells[0] = 88;
    t.rows[1].cells[1] = 30;

    exit t.diag() as u8;
}

:/ OUTPUT
@exit_code=42
/:
