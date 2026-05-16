: RUN_ASM :

start() {
    i32 value = 0;

    if 5 > 3; { value += 1; }
    if 2 == 2; { value += 2; }
    if 7 < 1; { value += 4; }
    if 8 >= 8; { value += 8; }
    if 3 != 4; { value += 16; }

    exit value as u8;
}

:/ OUTPUT
@exit_code=27
/:
