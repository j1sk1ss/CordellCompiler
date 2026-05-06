: RUN_ASM :
start() {
    i32 a = 1;
    i32 b = 0;
    @[counter(1000000)] loop {
        i32 tmp = a;
        a = a + b;
        b = tmp;
    }
    exit b;
}

:/ OUTPUT
@measure_time
@measure_lines
@repeat=5
@exit_code=187
/: