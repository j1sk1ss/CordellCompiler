: BLOCK_TEST :
: RUN_ASM :

container row_table {
    arr rows[2, ptr i32];

    @[self]
    function get(ptr row_table self, i32 r, i32 c) -> i32 {
        return self.rows[r][c];
    }
}

start() {
    arr left[3, i32]  = { 1,  2,  3  };
    arr right[3, i32] = { 10, 20, 30 };

    row_table t;
    t.rows[0] = ref left;
    t.rows[1] = ref right;

    exit (t.get(1 as i32, 2 as i32) + t.get(0 as i32, 1 as i32) + 10) as u8;
}

:/ OUTPUT
@exit_code=42
/:
