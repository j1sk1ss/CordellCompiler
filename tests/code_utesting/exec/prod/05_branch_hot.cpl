: RUN_ASM :
start() {
    i64 i = 0;
    i64 acc = 0;

    while i < 200000003; {
        if (i & 1) == 0; acc += 3;
        else acc += 1;

        i += 1;
    }

    exit acc & 0xFF;
}

:/ OUTPUT
@measure_time
@measure_lines
@repeat=5
@exit_code=7
/: