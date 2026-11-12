: RUN_ASM :

function finish(i32 code) -> i0 {
    exit code as u8;
}

start() {
    defer finish(17 as i32);
    exit 3;
}

:/ OUTPUT
@exit_code=17
/:
