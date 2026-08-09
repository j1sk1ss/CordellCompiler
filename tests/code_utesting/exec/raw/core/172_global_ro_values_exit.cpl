: BLOCK_TEST :
: RUN_ASM :

glob ro i32 A = 12;
glob ro i32 B = 30;

function sum() -> i32 {
    return A + B;
}

start() {
    exit sum() as u8;
}

:/ OUTPUT
@exit_code=42
/:
