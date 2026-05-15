: RUN_ASM :

start() {
    i32 zero = 0;
    i32 out = 0;

    if 1 || ((10 / zero) > 1); {
        out = 23;
    } else {
        out = 99;
    }

    exit out as u8;
}

:/ OUTPUT
@exit_code=23
/:
