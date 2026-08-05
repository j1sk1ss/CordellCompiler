: BLOCK_TEST :
: RUN_ASM :

start() {
    arr choices[2, ptr i8] = { ref "bad", ref "*ok" };
    ptr i8 selected = choices[1];

    exit selected[0] as u8;
}

:/ OUTPUT
@exit_code=42
/:
