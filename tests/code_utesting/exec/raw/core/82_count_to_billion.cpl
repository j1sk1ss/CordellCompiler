: RUN_ASM :
@[naked] start() {
    @[counter(1000000000)] loop {
    }
    exit 0;
}

:/ OUTPUT
@measure_time
@measure_lines
@repeat=5
@exit_code=0
/: