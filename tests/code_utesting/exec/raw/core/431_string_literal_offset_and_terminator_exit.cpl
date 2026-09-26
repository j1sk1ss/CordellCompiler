: BLOCK_TEST :
: RUN_ASM :

start() {
    ptr i8 text = ref "forty-two";
    ptr i8 tail = text + 6;
    i32 score = (tail[0] - 't') as i32;
    score += (tail[2] - 'o') as i32;
    score += (text[9] == 0) * 42;
    exit score as u8;
}

:/ OUTPUT
@exit_code=42
/:
