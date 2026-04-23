: RUN_ASM :
start() {
    i32 a = 1;
    i32 b = 2;
    i32 c = 3;
    i32 i = 0;

    while i < 10000000; {
        a = a + b * c;
        b = b + a / 3;
        c = c ^ (a - b);
        i += 1;
    }

    exit (a + b + c) as u8;
}

:/ OUTPUT
@measure_time
@measure_lines
@repeat=5
@exit_code=235
/: