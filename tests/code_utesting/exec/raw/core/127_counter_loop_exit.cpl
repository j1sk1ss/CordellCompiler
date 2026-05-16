: RUN_ASM :

start() {
    i32 sum = 0;

    @[counter(8)] loop {
        sum += 3;
    }

    exit sum as u8;
}

:/ OUTPUT
@exit_code=24
/:
