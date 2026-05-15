: RUN_ASM :

function mix(i32 a, i32 b = 7, i32 c = 3) -> i32 {
    return a * 2 + b - c;
}

start() {
    exit mix(9) as u8;
}

:/ OUTPUT
@exit_code=22
/:
