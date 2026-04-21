: RUN_ASM :
function step(i64 x) -> i64 {
    return ((x * 3) + 1) & 0xFF;
}

start() {
    i64 i = 0;
    i64 acc = 0;

    while i < 100000003; {
        acc = step(acc);
        i += 1;
    }

    exit acc;
}

:/ OUTPUT
@measure_time
@measure_lines
@repeat=5
@exit_code=13
/: