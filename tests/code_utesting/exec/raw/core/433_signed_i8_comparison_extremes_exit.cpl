: BLOCK_TEST :
: RUN_ASM :

start() {
    i8 minimum = -128;
    i8 maximum = 127;
    i32 score = 0;
    if minimum < maximum; { score += 20; }
    if maximum > 0; { score += 20; }
    if minimum != maximum; { score += 2; }
    exit score as u8;
}

:/ OUTPUT
@exit_code=42
/:
