: BLOCK_TEST :
: RUN_ASM :

container abi_cell {
    i32 value;
}

@[abi]
function fill_cell6(ptr abi_cell cell, i32 a, i32 b, i32 c, i32 d, i32 e, i32 f) -> i0 {
    cell.value = a + b + c + d + e + f;
}

start() {
    abi_cell cell;
    fill_cell6(ref cell, 1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32, 27 as i32);

    exit cell.value as u8;
}

:/ OUTPUT
@exit_code=42
/:
