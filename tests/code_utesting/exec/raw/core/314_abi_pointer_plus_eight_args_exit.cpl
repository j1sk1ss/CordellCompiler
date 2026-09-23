: BLOCK_TEST :
: RUN_ASM :

container abi_accumulator {
    i32 value;
}

@[abi]
function fill_cell8(ptr abi_accumulator cell, i32 a, i32 b, i32 c, i32 d, i32 e, i32 f, i32 g, i32 h) -> i0 {
    cell.value = a + b + c + d + e + f + g + h;
}

start() {
    abi_accumulator cell;
    fill_cell8(ref cell, 1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32, 6 as i32, 7 as i32, 14 as i32);

    exit cell.value as u8;
}

:/ OUTPUT
@exit_code=42
/:
