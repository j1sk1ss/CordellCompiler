: BLOCK_TEST :
: RUN_ASM :

function add428(ptr i32 value, i32 delta) -> i0 { 
    dref value += delta; 
}

function schedule428(ptr i32 value) -> i0 {
    defer add428(value, 40 as i32);
    dref value += 2;
}

start() {
    i32 value = 0 as i32;
    schedule428(ref value);
    exit value as u8;
}

:/ OUTPUT
@exit_code=42
/:
