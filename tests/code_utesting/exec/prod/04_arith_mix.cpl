: RUN_ASM :
start() {
    i64 i = 0;
    i64 acc = 1;

    while i < 200000000; {
        acc = ((acc * 13) + 7) & 0xFF;
        i += 1;
    }

    exit acc;
}

:/ OUTPUT
@measure_time
@measure_lines
@repeat=5
@exit_code=1
/: