: RUN_ASM :

function adjust(i32 a, i32 b = 12 as i32) -> i32;

function adjust(i32 a, i32 b = 12 as i32) -> i32 {
    return a + b * 2;
}

start() {
    exit adjust(5 as i32) as u8;
}

:/ OUTPUT
@exit_code=29
/:
