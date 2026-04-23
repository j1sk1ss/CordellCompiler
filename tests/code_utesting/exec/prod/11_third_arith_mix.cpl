: RUN_ASM :
function mix(i32 x, i32 y) -> i32 {
    return (x * 3 + y * 5) ^ (x - y);
}

start() {
    i32 i = 0;
    i32 acc = 0;

    while i < 5000000; {
        acc += mix(i, acc);
        i += 1;
    }

    exit acc as u8;
}

:/ OUTPUT
@measure_time
@measure_lines
@repeat=5
@exit_code=64
/: