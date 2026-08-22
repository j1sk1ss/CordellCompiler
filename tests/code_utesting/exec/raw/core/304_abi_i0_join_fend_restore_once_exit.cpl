: BLOCK_TEST :
: RUN_ASM :

container branch_cell {
    i32 value;
}

@[abi]
function choose_and_bump(ptr branch_cell cell, i32 input) -> i0 {
    if input > 10; cell.value = input;
    else cell.value = 10 as i32;

    cell.value += 1;
}

start() {
    branch_cell cell;
    choose_and_bump(ref cell, 41 as i32);

    exit cell.value as u8;
}

:/ OUTPUT
@exit_code=42
/:
